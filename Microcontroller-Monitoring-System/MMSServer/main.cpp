#include <Listeners.h>
#include <MMSServer.h>
#include <iostream>
#include <cstdlib>
#include "Arduino.h"
#include "Handlers.h"
#include "Watchdog.h"
#include <Settings.h>
#include <Tar.h>
#include <Compression.h>
#include <fstream>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::ofstream;

MMSServer* server = NULL;

struct ProcessArguments{
	MMSServer* server;
	const char* port;
	bool handleTimeout;
};

#ifdef _WIN32
#include <process.h>
unsigned __stdcall processThread(void* args){
	ProcessArguments* arguments = (ProcessArguments*)args;
	MMSServer* server = arguments->server;
	server->serverLoop(arguments->port, arguments->handleTimeout);
	_endthreadex(0);
	return 0;
}
#else
#include <pthread.h>
void* processThread(void* args){
	ProcessArguments* arguments = (ProcessArguments*)args;
	MMSServer* server = arguments->server;
	server->serverLoop(arguments->port, arguments->handleTimeout);
	cout << "Exiting" << endl;
	pthread_exit(NULL);
	return NULL;
}
#endif

bool echo(Message* message, MMSConnection* connection){
	string to_send = message->getParameters().front();
	if(to_send == "") to_send = "No parameters";
	vector<byte> to_send_vector(to_send.begin(), to_send.end());
	connection->handleWrite(to_send_vector, 0, "Server");
	return true;
}

bool defaultHandler(Message* message, MMSConnection* connection){
	string message_error = "Unknown command";
	vector<byte> message_to_send(message_error.begin(), message_error.end());
	connection->handleWrite(message_to_send, 0, "Server");
	return true;
}

int main(int argc, char* argv[]){
	/*Tar* tar = new Tar();
	Directory* dir = new Directory("CIC", "");
	tar->addDirectory(dir);
	vector<char> tar_contents = tar->outputTarData();

	ofstream ostr("CIC.tar", ios_base::out | ios_base::binary);
	for(int i = 0; i < tar_contents.size(); i++){
		ostr << tar_contents[i];
	}
	ostr.close();

	char* data = new char[tar_contents.size()];
	memcpy(data, &tar_contents[0], tar_contents.size());
	char* output = new char[tar_contents.size() + 1024];
	int output_size = tar_contents.size() + 1024;

	int rv = compress("bzip2", data, tar_contents.size(), output, output_size);
	
	ofstream ostr2("CIC.tar.bz2", ios_base::out | ios_base::binary);
	ostr2.write(output, output_size);
	ostr2.close();*/

#ifdef _WIN32
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
		cerr << "Unable to start WSAData" << endl;
		return 1;
	}
#endif
	server = new MMSServer();
	MainListener* listener = new MainListener();
	listener->initialize("MainHandler");
	listener->registerMessageHandler("echo", echo);
	listener->registerMessageHandler("authorize", authorize);
	listener->registerMessageHandler("hello", hello);
	listener->registerMessageHandler("addPinData", addPinData);
	listener->registerMessageHandler("resendLastMessage", resendLastMessage);
	listener->registerMessageHandler("ok", ok);
	listener->registerMessageHandler("getPinData", getPinData);
	listener->registerMessageHandler("getConnectedArduinos", getConnectedArduinos);
	listener->registerMessageHandler("getLogs", getLogs);
	listener->registerMessageHandler("getLogsByPin", getLogsByPin);
	listener->registerDefaultMessageHandler(defaultHandler);
	listener->registerConnectionHandler("2567", handleComputerConnection);
	listener->registerDisconnectionHandler("2568", handleArduinoDisconnection);
	server->registerConnectionListener(listener);
	server->registerDisconnectionListener(listener);
	server->setRunning(true);
#ifdef _WIN32
	ProcessArguments* computer_thread_arguments = new ProcessArguments();
	computer_thread_arguments->port = "2567";
	computer_thread_arguments->server = server;
	computer_thread_arguments->handleTimeout = false;
	uintptr_t computer_thread = _beginthreadex(NULL, 0, processThread, (void*)computer_thread_arguments, 0, NULL);
	ProcessArguments* arduino_thread_arguments = new ProcessArguments();
	arduino_thread_arguments->port = "2568";
	arduino_thread_arguments->server = server;
	arduino_thread_arguments->handleTimeout = true;
	uintptr_t arduino_thread = _beginthreadex(NULL, 0, processThread, (void*)arduino_thread_arguments, 0, NULL);
#else
	//REDO THIS TO UPDATE IT!!!!
	pthread_t arduino_thread, computer_thread, watchdog_thread;
	ProcessArguments* computer_thread_arguments = new ProcessArguments();
	computer_thread_arguments->port = "2567";
	computer_thread_arguments->server = server;
	computer_thread_arguments->handleTimeout = false;
	if(pthread_create(&computer_thread, NULL, processThread, (void*)computer_thread_arguments) != 0){
		return -1;
	}
	ProcessArguments* arduino_thread_arguments = new ProcessArguments();
	arduino_thread_arguments->port = "2568";
	arduino_thread_arguments->server = server;
	arduino_thread_arguments->handleTimeout = true;
	if(pthread_create(&arduino_thread, NULL, processThread, (void*)arduino_thread_arguments) != 0){
		pthread_join(computer_thread, NULL);
		return -1;
	}
	if(pthread_create(&watchdog_thread, NULL, watchdogThread, (void*)server) != 0){
		pthread_join(computer_thread, NULL);
		pthread_join(arduino_thread, NULL);
		return -1;
	}
#endif
	server->setRunning(false);

#ifdef _WIN32
	WaitForSingleObject((HANDLE)arduino_thread, INFINITE);
	WaitForSingleObject((HANDLE)computer_thread, INFINITE);
	CloseHandle((HANDLE)arduino_thread);
	CloseHandle((HANDLE)computer_thread);
#else
	pthread_join(arduino_thread, NULL);
	pthread_join(computer_thread, NULL);
	pthread_join(watchdog_thread, NULL);
#endif

#ifdef _WIN32
	WSACleanup();
#endif

	return 0;
}
