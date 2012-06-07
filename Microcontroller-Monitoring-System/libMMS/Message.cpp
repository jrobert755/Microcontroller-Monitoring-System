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

Message::Message(){
	m_size_of_header = 0;
	m_connection_type = 0;
	m_size_of_message = 0;
	m_is_empty = true;
	m_name = "";
	m_type_of_connection = 0;
}

bool Message::isEmpty(){
	return m_is_empty;
}

void Message::clear(){
	m_header_bytes.clear();
	m_message_bytes.clear();
	m_size_of_header = 0;
	m_connection_type = 0;
	m_size_of_message = 0;
	m_name = "";
	m_is_empty = true;
	m_type_of_connection = 0;
}

void Message::setSizeOfHeader(unsigned int sizeOfHeader){
	m_size_of_header = sizeOfHeader;
	m_is_empty = false;
}

bool Message::addHeaderBytes(vector<byte> headerBytes){
	m_header_bytes.insert(m_header_bytes.end(), headerBytes.begin(), headerBytes.end());
	if(m_header_bytes.size() == m_size_of_header){
		byte* header_bytes = &m_header_bytes[0];
		m_type_of_connection = header_bytes[0];
		int size_of_name = header_bytes[1];
		m_name = string((char*)&header_bytes[2], size_of_name);
		for(unsigned int i = size_of_name + 2; i < m_header_bytes.size(); i++){
			m_size_of_message += (header_bytes[i] << ((i - size_of_name + 2) * 8));
		}
	}
	return true;
}

bool Message::addMessageBytes(vector<byte> messageBytes){
	m_message_bytes.insert(m_message_bytes.end(), messageBytes.begin(), messageBytes.end());
	return true;
}

bool Message::isHeaderComplete(){
	return m_size_of_header == m_header_bytes.size();
}

bool Message::isMessageComplete(){
	return m_size_of_message == m_message_bytes.size();
}

int Message::sizeRemainingForHeader(){
	return m_size_of_header - m_header_bytes.size();
}

int Message::sizeRemainingForMessage(){
	return m_size_of_message - m_message_bytes.size();
}

vector<byte> Message::getMessage(){
	return m_message_bytes;
}

string Message::getName(){
	return m_name;
}

byte Message::getTypeOfConnection(){
	return m_type_of_connection;
}

void Message::setFunctionName(string functionName){
	m_function_name = functionName;
}

void Message::setParameters(vector<string> parameters){
	m_parameters = parameters;
}

string Message::getFunctionName(){
	return m_function_name;
}

vector<string> Message::getParameters(){
	return m_parameters;
}