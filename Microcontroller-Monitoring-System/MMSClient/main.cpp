#include <Listeners.h>
#include <iostream>
#include <cstdlib>
#include <process.h>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;

#ifdef _WIN32
unsigned __stdcall listenThread(void* args){
	MMSConnection* connection = (MMSConnection*)args;

	SOCKET connection_socket = connection->getSocket();
	SOCKET highest_socket;

	fd_set sockets_to_read;
	FD_ZERO(&sockets_to_read);
	while(connection->isConnected()){
		FD_SET(connection_socket, &sockets_to_read);
		highest_socket = connection_socket + 1;

		struct timeval timeout;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		int ports_waiting = select(highest_socket, &sockets_to_read, NULL, NULL, &timeout);
		if(ports_waiting > 0){
			if(FD_ISSET(connection_socket, &sockets_to_read)){
				connection->handleRead();
			}
		}
	}

 	_endthreadex(0);
	return 0;
}
#else
void* computerThread(void* args){
	ArduinoServer* server = (ArduinoServer*)args;
	server->clientLoop();
	return NULL;
}
#endif

bool echo(Message* message, MMSConnection* connection){
	cout << message->getParameters().front() << endl;
	return true;
}

bool defaultHandler(Message* message, MMSConnection* connection){
	cout << message->getFunctionName() << endl;
	return true;
}

int main(int argc, char* argv[]){
#ifdef _WIN32
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
		cerr << "Unable to start WSAData" << endl;
		return 1;
	}
#endif
	MainListener* listener = new MainListener();
	listener->initialize("MainListener");
	listener->registerMessageHandler("echo", echo);
	listener->registerDefaultMessageHandler(defaultHandler);
	MMSConnection* connection = new MMSConnection();
	connection->registerListener("MMSReadListener", listener);
	connection->registerListener("MMSWriteListener", listener);
	connection->connectToHost("localhost", "2567", false);
	MMSConnectEvent connectEvent(connection, "2567");
	listener->onConnectEvent(connectEvent);

#ifdef _WIN32
	uintptr_t computer_thread = _beginthreadex(NULL, 0, listenThread, (void*)connection, 0, NULL);
#else
	pthread_t computer_thread;
	if(pthread_create(&computer_thread, NULL, listenThread, (void*)connection) != 0){
		return -1;
	}
#endif

	string read_in_data = "";
	while(true){
		getline(cin, read_in_data);
		if(read_in_data == "end" || read_in_data == "quit") break;
		vector<byte> to_send(read_in_data.begin(), read_in_data.end());
		connection->handleWrite(to_send, 1, "Client");
	}

#ifdef _WIN32
	WSACleanup();
#endif

	return 0;
}