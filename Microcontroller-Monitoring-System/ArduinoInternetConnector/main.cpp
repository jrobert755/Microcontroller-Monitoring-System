#include "serialCommunication.h"
//#include <MMS.h>
//#include <MMSConnections.h>
//#include <Listeners.h>
#include <iostream>
#include <fstream>
//#include <pthread.h>
#include <string>
#include <sstream>
#include <ctime>

#include "HTTPConnection.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::string;
using std::ios_base;
using std::ostringstream;

int fd = 0;

string serial = "";
string path_to_file = "/home/jrobert755/googledrive/reading.csv";
int pins_to_get = 0;

void getReadings(){
	string query_string = "serial=" + serial;
	for(int i = 1; i < 64; i *= 2){
		if((pins_to_get & i) == i){
			query_string += "&pin";
			query_string += (char)(i+48);
		}
	}
	
	ofstream ostr(path_to_file.c_str(), ios_base::out | ios_base::binary);
	
}

/*bool defaultHandler(Message* message, MMSConnection* connection){
	vector<string> parameters = message->getParameters();
	string name = message->getFunctionName();
	string to_send = name;
	for(unsigned int i = 0; i < parameters.size(); i++){
		to_send += " ";
		to_send += parameters[i];
	}
	
	cout << "Received: " << to_send << endl;
	to_send += '\n';
	
	serialport_write(fd, to_send.c_str());
	return true;
}

void* serverListener(void* arguments){
	MMSConnection* connection = (MMSConnection*) arguments;
	
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
}*/

int main(int argc, char *argv[]) 
{
    char serialport[256];
    int baudrate = B9600;  // default
    char buf[256];
    int rc,n;
	string name;

    if (argc==1) {
        exit(EXIT_SUCCESS);
    }
	
	/*MMSConnection* connection = new MMSConnection();
	MainListener* listener = new MainListener();
	connection->connectToHost("localhost", "2568", false);
	MMSConnectEvent connectEvent(connection, "2568");
	listener->onConnectEvent(connectEvent);
	listener->registerDefaultMessageHandler(defaultHandler);*/

    /* parse options */
    int option_index = 0, opt;
    static struct option loptions[] = {
        {"help",       no_argument,       0, 'h'},
        {"port",       required_argument, 0, 'p'},
        {"baud",       required_argument, 0, 'b'},
        {"send",       required_argument, 0, 's'},
        {"receive",    no_argument,       0, 'r'},
        {"num",        required_argument, 0, 'n'},
        {"delay",      required_argument, 0, 'd'}
    };
    
    while(1) {
        opt = getopt_long (argc, argv, "hp:b:s:rn:d:",
                           loptions, &option_index);
        if (opt==-1) break;
        switch (opt) {
        case '0': break;
        case 'd':
            n = strtol(optarg,NULL,10);
            usleep(n * 1000 ); // sleep milliseconds
            break;
        case 'h':
            //usage();
            break;
        case 'b':
            baudrate = strtol(optarg,NULL,10);
            break;
        case 'p':
            strcpy(serialport,optarg);
            fd = serialport_init(optarg, baudrate);
            if(fd==-1) return -1;
            break;
        case 'n':
            n = strtol(optarg, NULL, 10); // convert string to number
            rc = serialport_writebyte(fd, (uint8_t)n);
            if(rc==-1) return -1;
            break;
        case 's':
            strcpy(buf,optarg);
            rc = serialport_write(fd, buf);
            if(rc==-1) return -1;
            break;
        case 'r':
			while(true){
				serialport_read_until(fd, buf, '\n');
				//if(rv == -1) continue;
				//sleep(2);
				printf("read: %s",buf);
			}
            break;
        }
    }
	
	/*pthread_t serverT;
	pthread_create(&serverT, NULL, serverListener, (void*)connection);*/
	
	HTTPConnection connection("localhost");
	//while(connection->isConnected()){
	while(true){
		serialport_read_until(fd, buf, '\n');
		if(strlen(buf) <= 1) continue;
		buf[strlen(buf)-1] = 0;
		/*if(strncmp(buf, "name:", 5) == 0){
			name = &buf[5];
			cout << name << endl;
			continue;
		}*/
		ofstream ostr("logging.log", std::ios_base::app);
		cout << "Arduino: " << buf << endl;
		ostr << "Arduino: " << buf << endl;
		/*vector<byte> to_send;
		for(unsigned int i = 0; i < strlen(buf); i++){
			to_send.push_back(buf[i]);
		}
		connection->handleWrite(to_send, 2, name);*/
		string read_in = buf;
		size_t position = read_in.find(';');
		if(position == string::npos) continue;
		string where = read_in.substr(0, position);
		string to_send = read_in.substr(position+1);
		string output;
		if(where == "newreading"){
			connection.sendMessage(Post, "/logger/current/newreading.php", to_send, output);
			size_t pos1 = 0;
			size_t pos2 = 0;
			string current_item;
			int pin = 0;
			double reading = 0;
			cout << "Here" << endl;
			while(pos1 != string::npos && pos1 < to_send.length()){
				pos2 = to_send.find('&', pos1);
				current_item = to_send.substr(pos1, pos2);
				cout << "Current item: " << current_item.c_str() << endl;
				if(current_item.compare(0, strlen("pin"), "pin") == 0){
					sscanf(current_item.c_str(), "pin=%d", &pin);
				} else if(current_item.compare(0, strlen("reading"), "reading") == 0){
					sscanf(current_item.c_str(), "reading=%lf", &reading);
				}
				pos1 = pos2 + 1;
				if(pos1 >= to_send.length()) pos1 = string::npos;
			}
			cout << "Here2" << endl;
			time_t cur_time = time(NULL);
			struct tm* tm_time = localtime(&cur_time);
			ostringstream output;
			char conv_time[1024];
			strftime(conv_time, 1024, "%b %d %Y %H:%M:%S", tm_time);
			output << "\"" << conv_time << "\",pin" << pin << "," << reading;
			string output_string = output.str();
			ofstream ostr("/home/jrobert755/test.csv", ios_base::out | ios_base::app);
			ostr << output_string.c_str() << endl;
			ostr.close();
		}
		else if(where =="update") connection.sendMessage(Post, "/logger/current/update.php", to_send, output);
		else continue;
		cout << "Server: " << endl << output;
		ostr << "Server: " << endl << output;
		ostr.close();
		
		//apache uses \r\n; i use \n
		position = output.find("\r\n\r\n");
		string to_arduino = output.substr(position+4);
		position = 0;
		size_t position2 = 0;
		while(position != string::npos && position < to_arduino.length()){
			position2 = to_arduino.find("\n", position);
			string temp = to_arduino.substr(position, position2+1);
			cout << "Wrote to arduino: " << temp << endl;
			serialport_write(fd, temp.c_str());
			position = position2;
			if(position != string::npos) position++;
		}
	}
	
	//pthread_join(serverT, NULL);

    exit(EXIT_SUCCESS);    
} // end main
    

