#include "HTTPConnection.h"

HTTPConnection::HTTPConnection(string address){
	m_address = address;
}

bool HTTPConnection::connect(){
	struct addrinfo hints, *servinfo, *p;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;    

	if (getaddrinfo(m_address.c_str(), "80", &hints, &servinfo) != 0) {
		return false;
	}
	
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((m_connection_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			continue;
		}

		if (::connect(m_connection_socket, p->ai_addr, p->ai_addrlen) == -1) {
			CloseConnection(m_connection_socket);
			continue;
		}
		break;
	}
	

	if (p == NULL) {
		return false;
	}

	freeaddrinfo(servinfo);
	return true;
}

void HTTPConnection::disconnect(){
	CloseConnection(m_connection_socket);
}

bool HTTPConnection::sendMessage(SendMethod method, string page, string body, string& received){
	string to_send = method == Get ? "GET " : "POST ";

	ostringstream to_send_stream;
	to_send_stream << to_send;
	to_send_stream << page;
	to_send_stream << " HTTP/1.1\n";
	to_send_stream << "HOST: " << m_address << "\n";
	to_send_stream << "Content-Type: application/x-www-form-urlencoded\n";
	to_send_stream << "Content-Length: " << body.length() << "\n\n";
	to_send_stream << body << "\n\n";

	to_send = to_send_stream.str();

	const char* to_send_char = to_send.c_str();
	if(!connect()) return false;
	int rv = send(m_connection_socket, to_send_char, strlen(to_send_char), 0);
	if(rv <= 0) return false;
	char recv_buffer[2048];
	memset(recv_buffer, 0, 2048);
	rv = recv(m_connection_socket, recv_buffer, 2048, 0);
	if(rv < 0) return false;
	received = recv_buffer;
	disconnect();
	return true;
}

string HTTPConnection::getHeader(){
	string header = "";
	char item[1];
	bool first_cr = false, first_nl = false, second_cr = false, second_nl = false;
	
	while(true){
		int rv = recv(m_connection_socket, item, 1, 0);
		if(rv < 0) break;
		if(item == '\r'){
			if(!first_cr){
				first_cr = true;
				first_nl = false;
				second_cr = false;
				second_nl = false;
			} else if(first_cr && first_nl){
				second_cr = true;
			} else{
				break;
			}
		} else if(item == '\n'){
			if(first_cr && !second_cr){
				first_nl = true;
				second_nl = false;
				header += item;
			} else if(first_cr && second_cr){
				header += item;
				break;
			} else{
				break;
			}
		} else{
			header += item;
		}
	}
	
	return header;
}
