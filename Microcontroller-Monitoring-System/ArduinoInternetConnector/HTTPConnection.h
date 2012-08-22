#ifndef _HTTPCONNECTION_H_
#define _HTTPCONNECTION_H_

#ifdef _WIN32
#define CloseConnection(x) closesocket(x)
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")
#else
#include <pthread.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#define CloseConnection(x) close(x)
#endif

#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>

using std::string;
using std::ostringstream;

enum SendMethod{
	Get,
	Post
};

class HTTPConnection{
public:
	HTTPConnection(string address);
	bool connect();
	void disconnect();
	
	bool sendMessage(SendMethod method, string page, string body, string& received);
	string getHeader();
private:
	string m_address;
	int m_connection_socket;
};

#endif
