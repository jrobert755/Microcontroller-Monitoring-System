#include "Handlers.h"

map<SOCKET, MMSConnection*>* computers = NULL;

bool handleComputerConnection(MMSConnection* connection, string port){
	if(computers == NULL) computers = server->getConnections(port);
	return true;
}

bool getConnectedArduinos(Message* message, MMSConnection* connection){
	if(known_arduinos.size() == 0) return true;
	string to_send = "connectedArduinos";
	for(map<string, Arduino*>::iterator iter = known_arduinos.begin(); iter != known_arduinos.end(); iter++){
		if(iter->second->isConnected()){
			to_send += " ";
			to_send += iter->first;
		}
	}

	vector<byte> to_send_vector;
	to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
	connection->handleWrite(to_send_vector, 0, "Server");
	return true;
}

bool getPinData(Message* message, MMSConnection* connection){
	vector<string> parameters = message->getParameters();
	if(parameters.size() != 5){
		string to_send = "Error: " + message->getFunctionName() + " takes 5 arguments";
		return true;
	}
	string arduino_name = parameters[0];

	map<string, Arduino*>::iterator arduino_iter = known_arduinos.find(arduino_name);
	Arduino* arduino;
	if(arduino_iter == known_arduinos.end()){
		arduino = new Arduino();
		arduino->setName(arduino_name);
		known_arduinos.insert(pair<string, Arduino*>(arduino_name, arduino));
	} else arduino = arduino_iter->second;

	string s_pin_number = parameters[1];
	string s_start_time = parameters[2];
	string s_end_time = parameters[3];
	string s_interval = parameters[4];

	int pin_number = atoi(s_pin_number.c_str());
	time_t start_time = (time_t)atoi(s_start_time.c_str());
	time_t end_time = (time_t)atoi(s_end_time.c_str());
	time_t interval = (time_t)atoi(s_interval.c_str());

	map<time_t, double> arduino_readings = arduino->getReadings(pin_number, start_time, end_time, interval);
	string s_arduino_readings = convertMapToString(arduino_readings);
	string to_send = "arduinoReadings ";
	to_send += arduino_name;
	to_send += " ";
	to_send += s_pin_number;
	to_send += " ";
	to_send += s_arduino_readings;
	vector<byte> to_send_vector;
	to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
	connection->handleWrite(to_send_vector, 0, "Server");
	return true;
}

bool getLogs(Message* message, MMSConnection* connection){
	vector<string> parameters = message->getParameters();
	if(parameters.size() != 1) return true;

	Directory* directory = new Directory(parameters[0], ".csv");
	Tar* tar = new Tar();
	tar->addDirectory(directory);
	vector<char> tar_data = tar->outputTarData();

	char* contents = new char[tar_data.size()];
	memcpy(contents, &tar_data[0], tar_data.size());
	char* output = new char[tar_data.size() + 1024];
	int output_size = tar_data.size() + 1024;
	int rv = compress("bzip2", contents, tar_data.size(), output, output_size);

	string to_send = "newFile ";
	vector<byte> to_send_vector;
	to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
	//to_send_vector.push_back('\"');

	for(int i = 0; i < output_size; i++){
		to_send_vector.push_back(output[i]);
	}
	//to_send_vector.push_back('\"');
	connection->handleWrite(to_send_vector, 0, "Server");
	delete directory;
	delete tar;
	delete[] contents;
	delete[] output;
	return true;
}

bool getLogsByPin(Message* message, MMSConnection* connection){
	vector<string> parameters = message->getParameters();
	if(parameters.size() != 1) return true;
	
	map<string, Arduino*>::iterator arduino_iter = known_arduinos.find(parameters[0]);
	Arduino* arduino;
	if(arduino_iter == known_arduinos.end()){
		arduino = new Arduino();
		arduino->setName(parameters[0]);
		known_arduinos.insert(pair<string, Arduino*>(parameters[0], arduino));
	} else arduino = arduino_iter->second;

	vector<string> arduino_contents = arduino->getAllReadingsByPin();

	File* temp_file = File::createBlankFile("logs.csv");
	temp_file->setVectorData(arduino_contents);
	Tar* tar = new Tar();
	tar->addFile(temp_file);
	vector<char> tar_data = tar->outputTarData();

	char* contents = new char[tar_data.size()];
	memcpy(contents, &tar_data[0], tar_data.size());
	char* output = new char[tar_data.size() + 1024];
	int output_size = tar_data.size() + 1024;
	int rv = compress("bzip2", contents, tar_data.size(), output, output_size);

	string to_send = "newFile ";
	vector<byte> to_send_vector;
	to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());

	for(int i = 0; i < output_size; i++){
		to_send_vector.push_back(output[i]);
	}
	connection->handleWrite(to_send_vector, 0, "Server");
	delete temp_file;
	delete tar;
	delete[] contents;
	delete[] output;

	return true;
}

bool setArduinoDigitalPin(Message* message, MMSConnection* connection){
	vector<string> parameters = message->getParameters();
	//NEED TO DO HANDLING OF IF THE MESSAGE IS WRONG!
	if(parameters.size() != 3) return true;

	map<string, Arduino*>::iterator arduino_iter = known_arduinos.find(parameters[0]);
	if(arduino_iter == known_arduinos.end()) return true;
	Arduino* arduino = arduino_iter->second;
	
	//NEED TO DO ERROR CHECKING TO MAKE SURE THE PIN IS IN THE CORRECT RANGE

	string to_send_string = "setDigitalPin ";
	to_send_string += parameters[1];
	to_send_string += " ";
	to_send_string += parameters[2];
	vector<byte> to_send;
	to_send.insert(to_send.end(), to_send_string.begin(), to_send_string.end());
	arduino->addNewMessage(to_send, 1, "Server");
	arduino->writeNextMessage();
	return true;
}