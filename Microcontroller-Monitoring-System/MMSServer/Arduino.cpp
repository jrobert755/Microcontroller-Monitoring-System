#include "Arduino.h"
#include "Resistor.h"
#include "Settings.h"

string convertHMStoString(time_t timeToConvert){
	//struct tm* local_time = new struct tm();
	//memset(local_time, 0, sizeof(tm));
	//localtime_s(local_time, &timeToConvert);
	struct tm tm_local_time;
	localtime_s(&tm_local_time, &timeToConvert);
	ostringstream ostr;
	if(tm_local_time.tm_hour < 10)
		ostr << "0";
	ostr <<  tm_local_time.tm_hour << ":";
	if(tm_local_time.tm_min < 10)
		ostr << "0";
	ostr << tm_local_time.tm_min << ":";
	if(tm_local_time.tm_sec < 10)
		ostr << "0";
	ostr << tm_local_time.tm_sec;
	return ostr.str();
}

time_t convertStringtoHMS(string timeToConvert){
	int hour, min, sec;
	const char* c_timeToConvert = timeToConvert.c_str();
	sscanf_s(c_timeToConvert, "%d:%d:%d", &hour, &min, &sec);
	/*struct tm* tm_time = new struct tm();
	memset(tm_time, 0, sizeof(tm));
	tm_time->tm_hour = hour;
	tm_time->tm_min = min;
	tm_time->tm_sec = sec;
	time_t time_t_time = mktime(tm_time);
	delete tm_time;*/
	struct tm tm_time;
	tm_time.tm_hour = hour;
	tm_time.tm_min = min;
	tm_time.tm_sec = sec;
	tm_time.tm_mon = 0;
	tm_time.tm_mday = 0;
	tm_time.tm_year = 0;
	time_t time_t_time = mktime(&tm_time);
	return time_t_time;
}

string convertMDYtoString(time_t timeToConvert){
	//struct tm* tm_local_time = new struct tm();
	//memset(tm_local_time, 0, sizeof(tm));
	//localtime_s(tm_local_time, &timeToConvert);
	struct tm tm_local_time;
	localtime_s(&tm_local_time, &timeToConvert);
	ostringstream ostr;
	ostr << tm_local_time.tm_mon + 1 << "." << tm_local_time.tm_mday << "." << (tm_local_time.tm_year - 100);
	//ostr << tm_local_time->tm_mon << "." << tm_local_time->tm_mday << "." << (tm_local_time->tm_year - 100);
	//delete tm_local_time;
	return ostr.str();
}

time_t convertStringtoMDY(string timeToConvert){
	int day, month, year;
	const char* c_timeToConvert = timeToConvert.c_str();
	sscanf_s(c_timeToConvert, "%d.%d.%d", &month, &day, &year);
	/*struct tm* tm_time = new struct tm();
	memset(tm_time, 0, sizeof(tm));
	tm_time->tm_mon = month;
	tm_time->tm_mday = day;
	tm_time->tm_year = year+100;
	time_t time_t_time = mktime(tm_time);
	delete tm_time;*/
	struct tm tm_time;
	tm_time.tm_mon = month-1;
	tm_time.tm_mday = day;
	tm_time.tm_year = year+100;
	tm_time.tm_hour = 0;
	tm_time.tm_min = 0;
	tm_time.tm_sec = 0;
	time_t time_t_time = mktime(&tm_time);
	return time_t_time;
}

bool ArduinoPinData::addNewReading(int pinNumber, time_t timeOfReading, double reading){
	map<time_t, double>* pin_readings;
	map<int, map<time_t, double> >::iterator pin_iter = m_pin_readings.find(pinNumber);
	if(pin_iter != m_pin_readings.end()) pin_readings = &pin_iter->second;
	else{
		map<time_t, double> to_insert;
		m_pin_readings.insert(pair<int, map<time_t, double> >(pinNumber, to_insert));
		pin_iter = m_pin_readings.find(pinNumber);
		pin_readings = &pin_iter->second;
	}
	pin_readings->insert(pair<time_t, double>(timeOfReading, reading));
	return true;
}

map<time_t, double> ArduinoPinData::getReadings(int pinNumber, time_t startTime, time_t endTime, time_t interval){
	map<time_t, double> return_map;
	map<int, map<time_t, double> >::iterator pin_iter = m_pin_readings.find(pinNumber);
	if(pin_iter == m_pin_readings.end()) return return_map;
	map<time_t, double> pin_readings = pin_iter->second;
	map<time_t, double>::iterator pin_readings_iter = pin_readings.begin();
	while(pin_readings_iter != pin_readings.end() && pin_readings_iter->first < startTime){
		pin_readings_iter++;
	}
	if(pin_readings_iter == pin_readings.end()) return return_map;
	while(pin_readings_iter != pin_readings.end() && (pin_readings_iter->first < endTime || endTime == 0)){
		/*if(return_vector.size() == 0) return_vector.push_back(pin_readings_iter->second);
		else{
			if(pin_readings_iter->second - return_vector.back() >= interval){
				return_vector.push_back(pin_readings_iter->second);
			}
		}*/
		if(return_map.size() == 0) return_map.insert(pair<time_t, double>(pin_readings_iter->first, pin_readings_iter->second));
		else{
			map<time_t, double>::iterator return_iter = return_map.end();
			return_iter--;
			if(pin_readings_iter->first - return_iter->first >= interval){
				return_map.insert(pair<time_t, double>(pin_readings_iter->first, pin_readings_iter->second));
			}
		}
		pin_readings_iter++;
	}
	return return_map;
}

vector<string> ArduinoPinData::getAllReadingsByPin(){
	map<time_t, map<int, double> > sorted_readings;
	for(map<int, map<time_t, double> >::iterator pin_iter = m_pin_readings.begin(); pin_iter != m_pin_readings.end(); pin_iter++){
		int pin = pin_iter->first;
		for(map<time_t, double>::iterator reading_iter = pin_iter->second.begin(); reading_iter != pin_iter->second.end(); reading_iter++){
			time_t reading_time = reading_iter->first;
			double reading = reading_iter->second;
			map<time_t, map<int, double> >::iterator sorted_iter = sorted_readings.find(reading_time);
			if(sorted_iter == sorted_readings.end()){
				map<int, double> to_insert;
				to_insert.insert(pair<int, double>(pin, reading));
				sorted_readings.insert(pair<time_t, map<int, double> >(reading_time, to_insert));
			} else{
				sorted_iter->second.insert(pair<int, double>(pin, reading));
			}
		}
	}

	vector<string> return_vector;
	for(map<time_t, map<int, double> >::iterator time_iter = sorted_readings.begin(); time_iter != sorted_readings.end(); time_iter++){
		string date = convertMDYtoString(time_iter->first);
		string time = convertHMStoString(time_iter->first);
		ostringstream ostr;
		ostr << "\"" << date << " " << time << "\",";
		int current_pin = 0;
		for(map<int, double>::iterator pin_iter = time_iter->second.begin(); pin_iter != time_iter->second.end(); pin_iter++){
			int pin = pin_iter->first;
			double reading = pin_iter->second;
			while(pin > current_pin){
				ostr << ',';
				current_pin++;
			}
			ostr << reading;
		}
		while(current_pin < 5){
			ostr << ',';
			current_pin++;
		}
		ostr << endl;
		string to_add = ostr.str();
		return_vector.push_back(to_add);
	}
	return return_vector;
}

void ArduinoPinData::addAllReadings(map<string, File*> files){
	for(map<string, File*>::iterator iter = files.begin(); iter != files.end(); iter++){
		string file_name = iter->first;
		if(file_name.find(".csv") == string::npos) continue;
		string sub_file_name = file_name.substr(0, file_name.find_last_of('.'));
		File* file = iter->second;
		//cout << "Getting mdy" << endl;
		time_t mdy = convertStringtoMDY(sub_file_name);
		//cout << "Got mdy" << endl;
		struct tm mdy_info;
		localtime_s(&mdy_info, &mdy);
		//Break the individual lines of the file into time, pin, reading
		vector<string> data = file->getVectorData();
		//cout << "Begin iterating through data" << endl;
		for(vector<string>::iterator vector_iter = data.begin(); vector_iter != data.end(); vector_iter++){
			int hour, min, sec;
			int pin;
			double reading;
			sscanf_s(vector_iter->c_str(), "%d:%d:%d,%*s %d,%lf", &hour, &min, &sec, &pin, &reading);
			//cout << "Got stuff: " << hour << " " << min << " " << sec << " " << pin << " " << reading << endl;
			struct tm time_struct;
			time_struct.tm_hour = hour;
			time_struct.tm_min = min;
			time_struct.tm_sec = sec;
			//cout << "Set hms" << endl;
			time_struct.tm_mday = mdy_info.tm_mday;
			time_struct.tm_mon = mdy_info.tm_mon;
			time_struct.tm_year = mdy_info.tm_year;
			//cout << "Set mdy" << endl;
			time_t int_time = mktime(&time_struct);
			//cout << "Got int_time" << endl;
			//int_time += mdy;
			map<time_t, double>* pin_readings;
			map<int, map<time_t, double> >::iterator pin_iter = m_pin_readings.find(pin);
			//cout << "Finding pin pointer" << endl;
			if(pin_iter != m_pin_readings.end()) pin_readings = &pin_iter->second;
			else{
				//cout << "Did not find, creating" << endl;
				map<time_t, double> to_insert;
				m_pin_readings.insert(pair<int, map<time_t, double> >(pin, to_insert));
				pin_iter = m_pin_readings.find(pin);
				pin_readings = &pin_iter->second;
			}
			//cout << "Found/created" << endl;
			pin_readings->insert(pair<time_t, double>(int_time, reading));
			//cout << "Inserted" << endl;
		}
	}
}

Arduino::Arduino(){
	m_connection = NULL;
	m_name = "";
	m_data = new ArduinoPinData();
	m_directory = NULL;
	m_nextPinReading = 1200;
	m_keepAlive = 15;
	m_pinsToRead = 63;
	m_processing = false;
	m_last_message_sent_sender = "";
	m_poll_curve = NULL;
	m_are_getting_poll_curve = false;
	m_is_connected = false;
}

Arduino::Arduino(MMSConnection* connection){
	m_connection = connection;
	m_name = "";
	m_data = new ArduinoPinData();
	m_directory = NULL;
	m_nextPinReading = 1200;
	m_keepAlive = 15;
	m_pinsToRead = 63;
	m_processing = false;
	m_last_message_sent_sender = "";
	m_poll_curve = NULL;
	m_are_getting_poll_curve = false;
	m_is_connected = connection->isConnected();
}

bool Arduino::setName(string name){
	m_name = name;
	if(m_directory != NULL){
		delete m_directory;
	}
	m_directory = new Directory(m_name, "");
	map<string, File*> reading_files = m_directory->getAllFiles();
	m_data->addAllReadings(reading_files);
	map<string, File*>::iterator iter = reading_files.find("settings.txt");
	if(iter != reading_files.end()){
		list<map<string, string> > settings = SettingsLoader::loadFile(iter->second->getName());
		for(list<map<string, string> >::iterator iter = settings.begin(); iter != settings.end(); iter++){
			map<string, string>::iterator map_iter = iter->find("name");
			string section_name = map_iter->second;
			if(section_name[section_name.length() - 1] == '\r') section_name = section_name.substr(0, section_name.length() -1 );
			if(section_name == "global"){
				for(map<string, string>::iterator settings_iter = iter->begin(); settings_iter != iter->end(); settings_iter++){
					string item = settings_iter->first;
					if(item == "pinsToUpdate"){
						m_pinsToRead = atoi(settings_iter->second.c_str());
					} else if(item == "keepAliveTime"){
						m_keepAlive = atoi(settings_iter->second.c_str());
					} else if(item == "nextPinReading"){
						m_nextPinReading = atoi(settings_iter->second.c_str());
					}
				}
			} else if(section_name == "PollCurve"){
				m_poll_curve = new PollCurve(*iter);
			} else if(section_name == "Resistor"){
				/* TODO: Do somethings with this */
				Resistor* resistor = new Resistor(*iter, this);
			}
		}
		//cout << "Done!" << endl;
	}
	return true;
}

bool Arduino::addNewReading(int pinNumber, time_t timeOfReading, double reading){
	string file_name = convertMDYtoString(timeOfReading);
	file_name += ".csv";
	File* output_file = m_directory->findFile(file_name);
	if(output_file == NULL){
		output_file = File::createBlankFile(m_directory->getName() + "/" + file_name);
		m_directory->addFile(output_file, file_name);
	}
	addReadingToFile(output_file, pinNumber, timeOfReading, reading);
	return m_data->addNewReading(pinNumber, timeOfReading, reading);
}

bool Arduino::addNewTemperatureReading(double temperatureReading){
	string file_name = "temperature";
	file_name += ".csv";
	File* output_file = m_directory->findFile(file_name);
	if(output_file == NULL){
		output_file = File::createBlankFile(m_directory->getName() + "/" + file_name);
		m_directory->addFile(output_file, file_name);
	}

	time_t current_time = time(NULL);
	
	ostringstream ostr;
	ostr << convertHMStoString(current_time) << "," << temperatureReading;
#ifdef _WIN32
	ostr << "\r";
#endif
	ostr << "\n";

	vector<string> file_contents = output_file->getVectorData();
	file_contents.push_back(ostr.str());
	output_file->setVectorData(file_contents);

	return true;
}

map<time_t, double> Arduino::getReadings(int pinNumber, time_t startTime, time_t endTime, time_t interval){
	return m_data->getReadings(pinNumber, startTime, endTime, interval);
}

vector<string> Arduino::getAllReadingsByPin(){
	return m_data->getAllReadingsByPin();
}

//TODO: Insert new reading according to pin, then time of reading
bool Arduino::addReadingToFile(File* file, int pinNumber, time_t timeOfReading, double reading){
	string string_reading_time = convertHMStoString(timeOfReading);
	ostringstream ostr;
	ostr << string_reading_time << ",pin " << pinNumber << "," << reading;
#ifdef _WIN32
	ostr << "\r";
#endif
	ostr << "\n";
	string to_insert = ostr.str();
	vector<string> file_readings = file->getVectorData();
	bool inserted = false;
	for(vector<string>::iterator iter = file_readings.begin(); iter != file_readings.end(); iter++){
		if(iter->compare(0, string_reading_time.length(), string_reading_time, 0, string_reading_time.length()) > 0){
			iter++;
			file_readings.insert(iter, to_insert);
			inserted = true;
			break;
		}
	}
	if(!inserted) file_readings.push_back(to_insert);
	file->setVectorData(file_readings);
	return true;
}

MMSConnection* Arduino::getConnection(){
	return m_connection;
}

void Arduino::setConnection(MMSConnection* connection){
	m_connection = connection;
}

bool Arduino::addNewMessage(vector<byte> messageToSend, byte senderConnectionType, string sender){
	m_message_queue_message.push(messageToSend);
	m_message_queue_sender_type.push(senderConnectionType);
	m_message_queue_sender.push(sender);
	return writeNextMessage();
}

bool Arduino::writeNextMessage(){
	if(m_message_queue_message.size() == 0 || m_processing) return true;
	if(!m_connection->isConnected()){
		while(m_message_queue_message.size() > 0){
			m_message_queue_message.pop();
			m_message_queue_sender_type.pop();
			m_message_queue_sender.pop();
		}
		return false;
	}

	m_last_message_sent_message = m_message_queue_message.front();
	m_message_queue_message.pop();

	m_last_message_sent_type = m_message_queue_sender_type.front();
	m_message_queue_sender_type.pop();

	m_last_message_sent_sender = m_message_queue_sender.front();
	m_message_queue_sender.pop();

	m_processing = true;
	return m_connection->handleWrite(m_last_message_sent_message, m_last_message_sent_type, m_last_message_sent_sender);
}

bool Arduino::resendLastMessage(){
	if(m_last_message_sent_sender == "") return true;
	return m_connection->handleWrite(m_last_message_sent_message, m_last_message_sent_type, m_last_message_sent_sender);
}

int Arduino::getNextPinReadingTime(){
	return m_nextPinReading;
}

int Arduino::getKeepAliveTime(){
	return m_keepAlive;
}

int Arduino::getPinsToRead(){
	return m_pinsToRead;
}

void Arduino::handledMessage(){
	m_processing = false;
}

PollCurve* Arduino::getPollCurveData(){
	return m_poll_curve;
}

void Arduino::resetConnection(MMSConnection* connection){
	m_processing = false;
	while(m_message_queue_message.size() > 0){
		m_message_queue_message.pop();
		m_message_queue_sender.pop();
		m_message_queue_sender_type.pop();
	}

	m_are_getting_poll_curve = false;
	m_connection = connection;
}

void Arduino::registerResistor(string name, Resistor* resistor){
	pair<map<string, Resistor*>::iterator, bool> return_val = m_resistors.insert(pair<string, Resistor*>(name, resistor));
	if(!return_val.second){
		delete return_val.first->second;
		return_val.first->second = resistor;
	}
}

void Arduino::reloadSettings(){
	map<string, File*> reading_files = m_directory->getAllFiles();
	map<string, File*>::iterator iter = reading_files.find("settings.txt");
	if(iter != reading_files.end()){
		iter->second->reloadFile();
		list<map<string, string> > settings = SettingsLoader::loadFile(iter->second->getName());
		for(list<map<string, string> >::iterator iter = settings.begin(); iter != settings.end(); iter++){
			map<string, string>::iterator map_iter = iter->find("name");
			string section_name = map_iter->second;
			if(section_name == "global"){
				for(map<string, string>::iterator settings_iter = iter->begin(); settings_iter != iter->end(); settings_iter++){
					string item = settings_iter->first;
					if(item == "pinsToUpdate"){
						m_pinsToRead = atoi(settings_iter->second.c_str());
					} else if(item == "keepAliveTime"){
						m_keepAlive = atoi(settings_iter->second.c_str());
					} else if(item == "nextPinReading"){
						m_nextPinReading = atoi(settings_iter->second.c_str());
					}
				}
			} else if(section_name == "PollCurve"){
				if(m_poll_curve != NULL) delete m_poll_curve;
				m_poll_curve = new PollCurve(*iter);
			} else if(section_name == "Resistor"){
				/* TODO: Do somethings with this */
				Resistor* resistor = new Resistor(*iter, this);
			}
		}
	}
}
