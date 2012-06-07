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

#include "MMSConnections.h"

MMSConnection::MMSConnection(){
	m_socket = -1;
	m_connected = false;
	m_mutex = new MMSMutex();

	m_read_listener = NULL;
	m_write_listener = NULL;
	m_last_heard_from = time(NULL);
}

MMSConnection::MMSConnection(SOCKET socket, sockaddr_storage& their_addr){
	m_socket = socket;
	m_connected = true;
	m_mutex = new MMSMutex();

	/*OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx = NULL;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);*/

	//MUST FIGURE OUT HOW TO FIX THIS
/*#ifdef _WIN32
	// Version < Vista
	if(bOsVersionInfoEx == NULL || osvi.dwMajorVersion < 6){*/
		struct sockaddr_in *sa = (struct sockaddr_in *)&their_addr;
		char* temp_address = inet_ntoa(sa->sin_addr);
		m_address = temp_address;
	/*} else{
#endif
		// Version > Vista
		char c_address[INET_ADDRSTRLEN];
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), c_address, sizeof c_address);
		m_address = c_address;
#ifdef _WIN32
	}
#endif*/

	m_read_listener = NULL;
	m_write_listener = NULL;
	m_last_heard_from = time(NULL);
}

MMSConnection::~MMSConnection(){
	delete m_mutex;
}

int MMSConnection::connectToHost(string host, string port, bool bindingMode){
	struct addrinfo hints, *servinfo, *p;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;    

	if (getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo) != 0) {
		return 1;
	}
	
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((m_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			continue;
		}

		if (connect(m_socket, p->ai_addr, p->ai_addrlen) == -1) {
			CloseConnection(m_socket);
			continue;
		}
		break;
	}
	

	if (p == NULL) {
		return 2;
	}

	/*OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx = NULL;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);*/

	//MUST FIGURE OUT HOW TO FIX THIS
/*#ifdef _WIN32
	// Version < Vista
	if(bOsVersionInfoEx == NULL || osvi.dwMajorVersion < 6){*/
		struct sockaddr_in *sa = (struct sockaddr_in *) p->ai_addr;
		char* temp_address = inet_ntoa(sa->sin_addr);
		m_address = temp_address;
	/*} else{
#endif
		// Version > Vista
		char c_address[INET_ADDRSTRLEN];
		inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), c_address, sizeof c_address);
		m_address = c_address;
#ifdef _WIN32
	}
#endif*/

	freeaddrinfo(servinfo);

	if(!bindingMode){
#ifdef _WIN32
		u_long iMode=1;
		ioctlsocket(m_socket, FIONBIO, &iMode);
#else
		fcntl(m_socket, F_SETFL, O_NONBLOCK);
#endif
	}

	m_connected = true;

	return 0;
}

int MMSConnection::beginListening(string port, bool bindingMode){
	struct addrinfo hints, *servinfo, *binded;
#ifdef _WIN32
	const char yes = 1;
#else
	int yes = 1;
#endif
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	int addr_info_return;
	if ((addr_info_return = getaddrinfo(NULL, port.c_str(), &hints, &servinfo)) != 0) {
		return 1;
	}
	
	for(binded = servinfo; binded != NULL; binded = binded->ai_next) {
		if ((m_socket = socket(binded->ai_family, binded->ai_socktype, binded->ai_protocol)) == -1) {
			continue;
		}

		if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			return 2;
		}

		if (bind(m_socket, binded->ai_addr, binded->ai_addrlen) == -1) {
			CloseConnection(m_socket);
			continue;
		}

		break;
	}
	
	if (binded == NULL)  {
		return 3;
	}

	if (listen(m_socket, 10) == -1) {
		return 4;
	}

	/*OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx = NULL;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);*/

	//MUST FIGURE OUT HOW TO FIX THIS
/*#ifdef _WIN32
	// Version < Vista
	if(bOsVersionInfoEx == NULL || osvi.dwMajorVersion < 6){*/
		m_address = "";
	/*} else{
#endif
		// Version > Vista
		char c_address[INET_ADDRSTRLEN];
		inet_ntop(binded->ai_family, get_in_addr((struct sockaddr *)binded->ai_addr), c_address, sizeof c_address);
		m_address = c_address;
#ifdef _WIN32
	}
#endif*/

	freeaddrinfo(servinfo);

	if(!bindingMode){
#ifdef _WIN32
		u_long iMode=1;
		ioctlsocket(m_socket, FIONBIO, &iMode);
#else
		fcntl(m_socket, F_SETFL, O_NONBLOCK);
#endif
	}

	m_connected = true;

	return 0;
}

SOCKET MMSConnection::getSocket(){
	return m_socket;
}

vector<byte> MMSConnection::mms_read(int bytesToRead, int& error){
	vector<byte> return_vector;
	unsigned char* holder = new unsigned char[bytesToRead];
	error = 0;
	int number_of_bytes_read = recv(m_socket, (char*)holder, bytesToRead, 0);
	if(number_of_bytes_read == 0){
		CloseConnection(m_socket);
		m_connected = false;
	} else if(number_of_bytes_read == -1){
#if defined(_WIN32)
		error = WSAGetLastError();
#elif defined(__linux__)
		error = errno;
#endif
	} else{
		m_last_heard_from = time(NULL);
		for(int i = 0; i < number_of_bytes_read; i++){
			return_vector.push_back(holder[i]);
		}
	}
	delete[] holder;
	return return_vector;
}

bool MMSConnection::mms_write(vector<byte> messageToSend){
	byte* message_to_send_data = &messageToSend[0];
	int remaining_to_send = messageToSend.size();
	while(remaining_to_send > 0){
		int amount_sent = send(m_socket, (char*)message_to_send_data, remaining_to_send, 0);
		message_to_send_data += amount_sent;
		remaining_to_send -= amount_sent;
	}
	return true;
}

int MMSConnection::registerListener(string listenerType, MMSListener* listener){
	if(listenerType == "MMSReadListener"){
		MMSReadListener* listener_cast = dynamic_cast<MMSReadListener*>(listener);
		if(listener_cast == 0) return -1;
		if(m_read_listener != NULL) return -1;
		m_read_listener = listener_cast;
	} else if(listenerType == "MMSWriteListener"){
		MMSWriteListener* listener_cast = dynamic_cast<MMSWriteListener*>(listener);
		if(listener_cast == 0) return -1;
		if(m_write_listener != NULL) return -1;
		m_write_listener = listener_cast;
	} else if(listenerType == "MMSDisconnectListener"){
		MMSDisconnectionListener* listener_cast = dynamic_cast<MMSDisconnectionListener*>(listener);
		if(listener_cast == 0) return -1;
		if(m_write_listener != NULL) return -1;
		m_disconnect_listener = listener_cast;
	}
	return 0;
}

bool MMSConnection::handleRead(){
	MMSReadEvent read_event(this);
	return m_read_listener->onReadEvent(read_event);
}

bool MMSConnection::handleWrite(vector<byte> messageToSend, byte senderConnectionType, string senderName){
	MMSWriteEvent write_event(this, messageToSend, senderConnectionType, senderName);
	return m_write_listener->onWriteEvent(write_event);
}

string MMSConnection::getAddress(){
	return m_address;
}

bool MMSConnection::isConnected(){
	return m_connected;
}

void MMSConnection::setConnected(bool connected){
	if(m_connected == true && connected == false){
		CloseConnection(m_socket);
	}
	m_connected = connected;
}

time_t MMSConnection::getLastHeardFrom(){
	return m_last_heard_from;
}