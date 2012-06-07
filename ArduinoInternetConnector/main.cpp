#include <SerialStream.h>
#include <iostream>
#include <MMS.h>
#include <MMSConnections.h>
#include <Listeners.h>
#include <pthread.h>

using std::cout;
using std::endl;

using namespace LibSerial;

SerialStream arduino;

void* serverListenThread(void* arguments){
	MMSConnection* connection = (MMSConnection*)arguments;
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
    pthread_exit(NULL);
    return NULL;
}

bool messageHandler(Message* message, MMSConnection* connection){
	return true;
}

int main(int argc, char* argv[]){
	MMSConnection* connection = new MMSConnection();
	connection->connectToHost("randomprojects.net", "2568", false);
	MainListener* listener = new MainListener();
	listener->registerDefaultMessageHandler(messageHandler);
	MMSConnectEvent connectEvent(connection, "2568");
	listener->onConnectEvent(connectEvent);
	arduino.Open("/dev/ttyUSB0");    
    arduino.SetBaudRate(SerialStreamBuf::BAUD_9600);  
    arduino.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
	while(connection->isConnected()){
		char buffer[512];
		memset(buffer, 0, 512);
		arduino.read(buffer, 512);
		cout << buffer << endl;
		/*vector<byte> to_send;
		for(unsigned int i = 0; i < strlen(buffer); i++){
			to_send.push_back(buffer[i]);
		}
		connection->handleWrite(to_send_vector, 2, "CIC");*/
	}
	return 0;
}
