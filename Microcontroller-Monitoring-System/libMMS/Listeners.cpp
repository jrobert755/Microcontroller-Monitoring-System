/* libMMS
 * Copyright 2012 Jean-Luc Roberts
 *
 * This file is part of the libMMS package.
 *
 * libMMS is free software: you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation, either version 3 of 
 * the License, or (at your option) any later version.
 *
 * libMMS is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with libMMS. If not, see 
 * <http://www.gnu.org/licenses/>.
 * 
 * A copy of the GNU Lesser General Public License can be found in
 * the parent directory for this directory.
 */

#include "Listeners.h"

#include <iostream>

using std::cout;
using std::endl;

MainListener::MainListener(){
	m_default_handler = NULL;
	m_name = "";
}

void MainListener::initialize(string listenerName){
	m_name = listenerName;
}

bool MainListener::onReadEvent(MMSReadEvent readEvent){
	MMSConnection* connection = readEvent.getConnection();
	SOCKET connection_socket = connection->getSocket();
	map<SOCKET, Message*>::iterator iter = m_messages.find(connection->getSocket());
	Message* connection_message = iter->second;
	int error = 0;
	if(connection_message->isEmpty()){
		vector<byte> message = connection->mms_read(1, error);
		if(!connection->isConnected() || error != 0){
			handleError(connection_socket);
			connection->setConnected(false);
			return true;
		}
		connection_message->setSizeOfHeader(message[0]);
	} else if(!connection_message->isHeaderComplete()){
		vector<byte> message = connection->mms_read(connection_message->sizeRemainingForHeader(), error);
		if(!connection->isConnected() || error != 0){
			handleError(connection_socket);
			connection->setConnected(false);
			return true;
		}
		connection_message->addHeaderBytes(message);
		if(connection_message->isHeaderComplete()){
			map<SOCKET, MMSConnection*>::iterator iter = m_unnamed_connections.find(connection_socket);
			if(iter != m_unnamed_connections.end() && connection_message->getName() != ""){
				m_unnamed_connections.erase(iter);
				pair<map<string, MMSConnection*>::iterator, bool> exists = m_connections.insert(pair<string, MMSConnection*>(connection_message->getName(), connection));
				if(!exists.second){
					MMSConnection* temp_connection = exists.first->second;
					m_unnamed_connections.insert(pair<SOCKET, MMSConnection*>(temp_connection->getSocket(), temp_connection));
					m_connections.erase(exists.first);
					m_connections.insert(pair<string, MMSConnection*>(connection_message->getName(), connection));
				}
			}
		}
	} else{
		vector<byte> message = connection->mms_read(connection_message->sizeRemainingForMessage(), error);
		if(!connection->isConnected() || error != 0){
			handleError(connection_socket);
			connection->setConnected(false);
			return true;
		}
		connection_message->addMessageBytes(message);
		if(connection_message->isMessageComplete()){
			vector<byte> completed_message = connection_message->getMessage();

			string debug_output;
			for(unsigned int i = 0; i < completed_message.size(); i++){
				debug_output += completed_message[i];
			}
			cout << "Received from " << connection->getAddress() << ":" << debug_output << endl;

			unsigned int size;
			for(size = 0; size < completed_message.size(); size++){
				if(completed_message[size] == ' ') break;
			}
			

			string function_name = string((char*)(&completed_message[0]), size);
			string parameters = "";
			int left_over = (completed_message.size() - size - 1);
			if(left_over > 0){
				parameters = string((char*)(&completed_message[size+1]), (completed_message.size() - size - 1));
			}

			string current_string = "";
			vector<string> parameters_vector;
			//bool quotes = false;
			for(string::iterator string_iter = parameters.begin(); string_iter != parameters.end(); string_iter++){
				if(*string_iter == ' '/* && !quotes*/){
					parameters_vector.push_back(current_string);
					current_string = "";
				} /*else if(*string_iter == '\"'){
					quotes = !quotes;
				} */else{
					current_string += *string_iter;
				}
			}
			if(current_string != "") parameters_vector.push_back(current_string);
			connection_message->setParameters(parameters_vector);
			connection_message->setFunctionName(function_name);

			map<string, list<MessageHandler> >::iterator message_iter = m_message_handlers.find(function_name);

			if(message_iter == m_message_handlers.end()){
				//string temporary = function_name + " " + parameters;
				m_default_handler(connection_message, connection);
			} else{
				for(list<MessageHandler>::iterator handler_iter = message_iter->second.begin(); handler_iter != message_iter->second.end(); handler_iter++){
					if((*handler_iter)(connection_message, connection)) break;
				}
				if(!connection->isConnected() || error != 0){
					handleError(connection_socket);
					connection->setConnected(false);
				}
			}

			connection_message->clear();
		}
	}
	return true;
}

bool MainListener::onWriteEvent(MMSWriteEvent writeEvent){
	MMSConnection* connection = writeEvent.getConnection();

	vector<byte> message = writeEvent.getMessage();
	int size_of_message = message.size();
	vector<byte> temporary_message_size_holder;
	int number_of_bytes_needed = 0;
	int temporary_size_of_message = size_of_message;
	while(temporary_size_of_message > 0){
		temporary_size_of_message >>= 8;
		number_of_bytes_needed++;
	}

	for(int i = 0; i < number_of_bytes_needed; i++){
		int temp_size = ((size_of_message&(0xFF<<(8*i)))>>(8*i));
		temporary_message_size_holder.push_back(temp_size);
	}

	vector<byte> temporary_holder;
	string name = writeEvent.getSenderName();
	byte size_of_name = name.size();
	byte connection_type = writeEvent.getSenderConnectionType();
	temporary_holder.push_back(connection_type);
	temporary_holder.push_back(size_of_name);
	temporary_holder.insert(temporary_holder.end(), name.begin(), name.end());
	temporary_holder.insert(temporary_holder.end(), temporary_message_size_holder.begin(), temporary_message_size_holder.end());

	vector<byte> to_send;
	to_send.push_back(temporary_holder.size());
	to_send.insert(to_send.end(), temporary_holder.begin(), temporary_holder.end());
	to_send.insert(to_send.end(), message.begin(), message.end());
	return connection->mms_write(to_send);
}

bool MainListener::onConnectEvent(MMSConnectEvent connectEvent){
	MMSConnection* connection = connectEvent.getConnection();
	connection->registerListener("MMSReadListener", this);
	connection->registerListener("MMSWriteListener", this);
	SOCKET connection_socket = connection->getSocket();
	m_unnamed_connections.insert(pair<SOCKET, MMSConnection*>(connection_socket, connection));
	Message* connection_messages = new Message();
	m_messages.insert(pair<SOCKET, Message*>(connection_socket, connection_messages));
	string port = connectEvent.getPort();
	map<string, list<ConnectionHandler> >::iterator iter = m_connection_handlers.find(port);
	if(iter != m_connection_handlers.end()){
		for(list<ConnectionHandler>::iterator function_iter = iter->second.begin(); function_iter != iter->second.end(); function_iter++){
			(*function_iter)(connection, port);
		}
	}
	return true;
}

bool MainListener::onDisconnectEvent(MMSDisconnectEvent disconnectEvent){
	MMSConnection* connection = disconnectEvent.getConnection();
	handleError(connection->getSocket());
	string port = disconnectEvent.getPort();
	map<string, list<DisconnectionHandler> >::iterator iter = m_disconnection_handlers.find(port);
	if(iter != m_disconnection_handlers.end()){
		for(list<DisconnectionHandler>::iterator function_iter = iter->second.begin(); function_iter != iter->second.end(); function_iter++){
			(*function_iter)(connection, port);
		}
	}
	return true;
}

bool MainListener::registerMessageHandler(string functionName, MessageHandler functionPointer){
	list<MessageHandler> handlers;
	map<string, list<MessageHandler> >::iterator iter = m_message_handlers.find(functionName);
	if(iter != m_message_handlers.end()) handlers = iter->second;
	handlers.push_back(functionPointer);
	if(iter == m_message_handlers.end()){
		m_message_handlers.insert(pair<string, list<MessageHandler> >(functionName, handlers));
	}
	return true;
}

bool MainListener::registerDefaultMessageHandler(MessageHandler functionPointer){
	if(m_default_handler != NULL) return false;
	m_default_handler = functionPointer;
	return true;
}

bool MainListener::registerConnectionHandler(string port, ConnectionHandler functionPointer){
	list<ConnectionHandler> handlers;
	map<string, list<ConnectionHandler> >::iterator iter = m_connection_handlers.find(port);
	if(iter != m_connection_handlers.end()) handlers = iter->second;
	handlers.push_back(functionPointer);
	if(iter == m_connection_handlers.end()){
		m_connection_handlers.insert(pair<string, list<ConnectionHandler> >(port, handlers));
	}
	return true;
}

bool MainListener::registerDisconnectionHandler(string port, DisconnectionHandler functionPointer){
	list<DisconnectionHandler> handlers;
	map<string, list<DisconnectionHandler> >::iterator iter = m_disconnection_handlers.find(port);
	if(iter != m_disconnection_handlers.end()) handlers = iter->second;
	handlers.push_back(functionPointer);
	if(iter == m_disconnection_handlers.end()){
		m_disconnection_handlers.insert(pair<string, list<DisconnectionHandler> >(port, handlers));
	}
	return true;
}

void MainListener::handleError(SOCKET socket){
	map<SOCKET, MMSConnection*>::iterator unnamed_iter = m_unnamed_connections.find(socket);
	if(unnamed_iter != m_unnamed_connections.end()){
		m_unnamed_connections.erase(unnamed_iter);
	}
	/*for(map<string, MMSConnection*>::iterator named_iter = m_connections.begin(); named_iter != m_connections.end(); named_iter++){
		if(named_iter->second->getSocket() == socket){
			m_connections.erase(named_iter);
		}
	}*/
	map<string, MMSConnection*>::iterator named_iter = m_connections.begin();
	while(named_iter != m_connections.end()){
		if(named_iter->second->getSocket() == socket){
			m_connections.erase(named_iter++);
		}
	}

	map<SOCKET, Message*>::iterator message_iter = m_messages.find(socket);
	if(message_iter != m_messages.end()){
		delete message_iter->second;
		m_messages.erase(message_iter);
	}
}

MMSConnection* MainListener::findConnection(string name){
	map<string, MMSConnection*>::iterator iter = m_connections.find(name);
	if(iter == m_connections.end()) return NULL;
	return iter->second;
}