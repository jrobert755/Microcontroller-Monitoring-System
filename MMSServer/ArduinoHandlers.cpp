#include "Handlers.h"
#include "Resistor.h"

map<string, Arduino*> known_arduinos;

Arduino* findArduino(string arduinoName, MMSConnection* connection){
	map<string, Arduino*>::iterator arduino_iter = known_arduinos.find(arduinoName);
	
	Arduino* arduino;
	if(arduino_iter == known_arduinos.end()){
		arduino = new Arduino(connection);
		arduino->setName(arduinoName);
		known_arduinos.insert(pair<string, Arduino*>(arduinoName, arduino));
		return arduino;
	} else return arduino_iter->second;
}

void sendGetPinReadingCommand(Arduino* arduino){
	ostringstream next_reading;
	string to_send;
	vector<byte> to_send_vector;

	PollCurve* poll_curve = arduino->getPollCurveData();
	time_t current_time = time(NULL);
	struct tm tm_current_time;
	localtime_s(&tm_current_time, &current_time);
	byte current_day = 1;
	for(int i = 0; i < tm_current_time.tm_wday; i++){
		current_day *= 2;
	}
	struct tm* hms = new struct tm();
	memset(hms, 0, sizeof(tm));
	hms->tm_hour = tm_current_time.tm_hour;
	hms->tm_min = tm_current_time.tm_min;
	hms->tm_sec = tm_current_time.tm_sec;
	time_t int_hms = mktime(hms);

	if(poll_curve == NULL || (current_day & poll_curve->getDaysToRun()) != current_day || int_hms >= poll_curve->getStartTime() || !arduino->gettingPollCurveData() || int_hms - poll_curve->getStartTime() >120){
		next_reading << "nextReading " << arduino->getNextPinReadingTime() << " " << arduino->getPinsToRead() << " 0";
		to_send = next_reading.str();
		to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
		arduino->addNewMessage(to_send_vector, 0, "Server");
	} else{
		int next_resistance = poll_curve->getNextResistance();
		if(next_resistance == -1){
			poll_curve->resetPollCurve();
			arduino->stopGettingPollCurveData();
			next_reading << "nextReading " << arduino->getNextPinReadingTime() << " " << arduino->getPinsToRead() << " 0";
			to_send = next_reading.str();
			to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
			arduino->addNewMessage(to_send_vector, 1, "Server");
		} else{
			if(!arduino->gettingPollCurveData()) arduino->startGettingPollCurveData();

			int* slave_pins, *resistors, *resistances;
			slave_pins = NULL;
			resistors = NULL;
			resistances = NULL;
			Resistor* first_resistor = arduino->getMainResistor();
			int number_of_resistors;
			first_resistor->calculateStepNeeded(next_resistance, number_of_resistors, &slave_pins, &resistors, &resistances);
			for(int i = 0; i < number_of_resistors; i++){
				ostringstream set_resistance;
				set_resistance << "setResistance " << slave_pins[i] << " " << resistors[i] << " " << resistances[i];
				to_send = set_resistance.str();
				to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
				arduino->addNewMessage(to_send_vector, 1, "Server");
				to_send_vector.clear();
			}

			next_reading << "nextReading " << arduino->getNextPinReadingTime() << " " << arduino->getPinsToRead() << " 1140";
			to_send = next_reading.str();
			to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
			arduino->addNewMessage(to_send_vector, 1, "Server");
		}
	}
}

bool handleArduinoConnection(MMSConnection* connection, string port){
	if(port != "2568") return false;
	cout << "New arduino connection" << endl;
	/*string to_send = "authorize";
	vector<byte> to_send_vector;
	to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
	connection->handleWrite(to_send_vector, 0, "Server");*/
	return true;
}

bool handleArduinoDisconnection(MMSConnection* connection, string port){
	if(port != "2568") return false;
	for(map<string, Arduino*>::iterator iter = known_arduinos.begin(); iter != known_arduinos.end(); iter++){
		if(iter->second->getConnection()->getSocket() == connection->getSocket()){
			iter->second->setConnected(false);
			string to_send = "disconnectedArduino ";
			to_send += iter->first;
			vector<byte> to_send_vector;
			to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
			for(map<SOCKET, MMSConnection*>::iterator map_iter = computers->begin(); map_iter != computers->end(); map_iter++){
				map_iter->second->handleWrite(to_send_vector, 0, "Server");
			}
			break;
		}
	}
	return true;
}

bool hello(Message* message, MMSConnection* connection){
	if(message->getTypeOfConnection() != 2) return false;
	string to_send = "authorize";
	vector<byte> to_send_vector;
	to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
	connection->handleWrite(to_send_vector, 1, "Server");
	return true;
}

bool authorize(Message* message, MMSConnection* connection){
	if(message->getTypeOfConnection() != 2) return false;
	string arduino_name = message->getName();
	map<string, Arduino*>::iterator iter = known_arduinos.find(arduino_name);
	Arduino* arduino;
	if(iter == known_arduinos.end() || !iter->second->isConnected()){
		if(computers != NULL){
			string to_send = "newArduinoConnection ";
			to_send += arduino_name;
			vector<byte> to_send_vector;
			to_send_vector.insert(to_send_vector.begin(), to_send.begin(), to_send.end());
			for(map<SOCKET, MMSConnection*>::iterator computer_connection_iter = computers->begin(); computer_connection_iter != computers->end(); computer_connection_iter++){
				computer_connection_iter->second->handleWrite(to_send_vector, 0, "Server");
			}
		}
	}

	//cout << "New Arduino connection" << endl;
	if(iter != known_arduinos.end()){
		//if(iter->second->getConnection()->isConnected()) return true;
		//iter->second->setConnection(connection);
		//return true;
		iter->second->resetConnection(connection);
		arduino = iter->second;
	} else{
		arduino = new Arduino(connection);
		//cout << "Setting name" << endl;
		arduino->setName(arduino_name);
		//cout << "Done setting name" << endl;
		known_arduinos.insert(pair<string, Arduino*>(arduino_name, arduino));
	}
	arduino->setConnected(true);

	//cout << "Setting keep alive" << endl;
	ostringstream keep_alive;

	keep_alive << "setKeepAlive " << arduino->getKeepAliveTime();
	string to_send = keep_alive.str();
	vector<byte> to_send_vector;
	to_send_vector.insert(to_send_vector.end(), to_send.begin(), to_send.end());
	arduino->addNewMessage(to_send_vector, 1, "Server");
	to_send_vector.clear();

	//cout << "Sending which pins to read from" << endl;
	sendGetPinReadingCommand(arduino);
	//cout << "Done" << endl;

	return true;
}

bool ok(Message* message, MMSConnection* connection){
	string arduino_name = message->getName();
	
	Arduino* arduino = findArduino(arduino_name, connection);
	arduino->handledMessage();
	arduino->writeNextMessage();
	return true;
}

bool addPinData(Message* message, MMSConnection* connection){
	if(message->getTypeOfConnection() != 2) return false;
	string arduino_name = message->getName();
	
	Arduino* arduino = findArduino(arduino_name, connection);
	arduino->handledMessage();

	time_t current_time = time(NULL);
	vector<string> parameters = message->getParameters();
	for(vector<string>::iterator iter = parameters.begin(); iter != parameters.end(); iter++){
		string current_line = (*iter);
		int pin_number;
		double reading;
		int number_of_times_read;
		sscanf_s(current_line.c_str(), "%d=%lf,%d", &pin_number, &reading, &number_of_times_read);
		arduino->addNewReading(pin_number, current_time, reading);
	}

	sendGetPinReadingCommand(arduino);

	return true;
}

bool resendLastMessage(Message* message, MMSConnection* connection){
	if(message->getTypeOfConnection() != 2) return false;
	string arduino_name = message->getName();
	
	Arduino* arduino = findArduino(arduino_name, connection);
	arduino->handledMessage();

	return arduino->resendLastMessage();
}