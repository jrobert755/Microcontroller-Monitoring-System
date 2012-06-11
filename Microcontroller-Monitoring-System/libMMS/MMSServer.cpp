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

#include "MMSServer.h"
#include <iostream>

#ifndef _WIN32
#define localtime_s(x, y) localtime_r(y, x)
#endif

using std::cout;
using std::endl;

#ifdef _WIN32
#pragma warning(disable:4244)
#endif

MMSServer::MMSServer(){
	m_connection_listener = NULL;
	m_disconnection_listener = NULL;
}

MMSServer::~MMSServer(){
	//if(m_connection_listener != NULL) delete m_connection_listener;
}

bool MMSServer::registerConnectionListener(MMSConnectionListener* connectionListener){
	if(m_connection_listener != NULL) return false;
	m_connection_listener = connectionListener;
	return true;
}

bool MMSServer::registerDisconnectionListener(MMSDisconnectionListener* disconnectionListener){
	if(m_disconnection_listener != NULL) return false;
	m_disconnection_listener = disconnectionListener;
	return true;
}

void MMSServer::handleConnect(MMSConnection* newConnection, string port){
	MMSConnectEvent connectEvent(newConnection, port);
	m_connection_listener->onConnectEvent(connectEvent);
}

int MMSServer::serverLoop(string port, bool handleTimeout){
	if(m_ports.find(port) != m_ports.end()) return -1;
	m_ports.insert(port);
	map<SOCKET, MMSConnection*> connections;
	m_connections.insert(pair<string, map<SOCKET, MMSConnection*>*>(port, &connections));
	MMSConnection* port_connection = new MMSConnection();
	int listening_status = port_connection->beginListening(port, false);
	if(listening_status != 0){
		cout << "Error listening on port " << port << endl;
		return listening_status;
	}
	SOCKET port_connection_socket = port_connection->getSocket();
	SOCKET highest_socket;

	fd_set sockets_to_read;
	FD_ZERO(&sockets_to_read);
	while(running){
		FD_SET(port_connection_socket, &sockets_to_read);
		highest_socket = port_connection_socket + 1;
		map<SOCKET, MMSConnection*>::iterator iter = connections.begin();
		while(iter != connections.end()){
			if(iter->second->isConnected()){
				SOCKET socket = iter->second->getSocket();
				FD_SET(socket, &sockets_to_read);
				if(highest_socket < (socket + 1)) highest_socket = socket + 1;
				++iter;
			} else{
				connections.erase(iter++);
			}
		}
		struct timeval timeout;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		int ports_waiting = select(highest_socket, &sockets_to_read, NULL, NULL, &timeout);
		if(ports_waiting > 0){
			map<SOCKET, MMSConnection*>::iterator iter = connections.begin();
			while(iter != connections.end()){
				if(FD_ISSET(iter->first, &sockets_to_read)){
					iter->second->handleRead();
					if(!iter->second->isConnected()){
						MMSDisconnectEvent disconnectEvent(iter->second, port);
						m_disconnection_listener->onDisconnectEvent(disconnectEvent);
						FD_CLR(iter->first, &sockets_to_read);
						delete iter->second;
						cout << "Disconnected" << endl;
						connections.erase(iter++);
					} else iter++;
				} else iter++;
			}
			if(FD_ISSET(port_connection_socket, &sockets_to_read)){
				struct sockaddr_storage their_addr;
				socklen_t sin_size = sizeof(their_addr);
				SOCKET connected_socket = accept(port_connection_socket, (struct sockaddr *)&their_addr, &sin_size);
				if (connected_socket == -1) {
					break;
				}
				cout << "New connection" << endl;
				MMSConnection* new_connection = new MMSConnection(connected_socket, their_addr);
				handleConnect(new_connection, port);
				connections.insert(pair<SOCKET, MMSConnection*>(connected_socket, new_connection));
			}
		}
		time_t current_time = time(NULL);
		map<SOCKET, MMSConnection*>::iterator connection_iter = connections.begin();
		while(handleTimeout && connection_iter != connections.end()){
			if((connection_iter->second->getLastHeardFrom() + 30) <= current_time){
				connection_iter->second->setConnected(false);
				MMSDisconnectEvent disconnectEvent(connection_iter->second, port);
				m_disconnection_listener->onDisconnectEvent(disconnectEvent);
				FD_CLR(connection_iter->first, &sockets_to_read);
				delete connection_iter->second;
				connections.erase(connection_iter++);
				cout << "Disconnected" << endl;
			} else{
				connection_iter++;
			}
		}
		time_t temp_time = time(NULL);
		if(temp_time > (current_time + 60*30)){
			struct tm temp_struct;
			localtime_s(&temp_struct, &temp_time);
			cout << "<" << asctime(&temp_struct) << "> Loop on port " << port << " still running" << endl;
			current_time = temp_time;
		}
	}
	//TODO: Clean up all connections
	map<string, map<SOCKET, MMSConnection*>*>::iterator iter = m_connections.find(port);
	if(iter != m_connections.end()) m_connections.erase(iter);
	for(map<SOCKET, MMSConnection*>::iterator iter = connections.begin(); iter != connections.end(); iter++){
		MMSDisconnectEvent disconnectEvent(iter->second, port);
		m_disconnection_listener->onDisconnectEvent(disconnectEvent);
		CloseConnection(iter->first);
		delete iter->second;
	}
	return 0;
}

set<string> MMSServer::getPorts(){
	return m_ports;
}

map<SOCKET, MMSConnection*>* MMSServer::getConnections(string port){
	map<string, map<SOCKET, MMSConnection*>*>::iterator iter = m_connections.find(port);
	if(iter == m_connections.end()) return NULL;
	else return iter->second;
}
