#include "Simulation.h"

void SimulationPin::setPinValues(int value, string response, string valueType){
	if(valueType == "Upper"){
		m_high_value = value;
		m_high_response = response;
	} else if(valueType == "Lower"){
		m_low_value = value;
		m_low_response = response;
	} else if(valueType == "Startpoint"){
		m_startpoint = value;
		m_startpoint_response = response;
	}
}

void SimulationPin::setPin(int pin){
	m_pin = pin;
}

void SimulationPin::setParameter(string parameter){
	m_parameter = parameter;
}

void SimulationPin::setNickname(string nickname){
	m_nickname = nickname;
}

void SimulationPin::setAlarm(bool alarm, string alarmResponse){
	m_alarm = alarm;
	m_alarm_response = alarmResponse;
}

void SimulationPin::setRecordingFrequency(string recordingFrequency){
	m_record_frequency = recordingFrequency;
}

Simulation::Simulation(list<map<string, string> > settings, vector<byte> fileContents){
	for(list<map<string, string> >::iterator iter = settings.begin(); iter != settings.end(); iter++){
		map<string, string> local_settings = *iter;
		
		map<string, string>::iterator name_iter = local_settings.find("name");
		if(name_iter == local_settings.end()) continue;
		
		string name = name_iter->first;
		transform(name.begin(), name.end(), name.begin(), ::tolower);
		if(name == "simulation"){
			m_output_directory = ".";
			m_output_file = "log.txt";
			m_start = 0, m_end = 0;
			m_user = "";
			
			for(map<string, string>::iterator map_iter = local_settings.begin(); map_iter != local_settings.end(); map_iter++){
				string name = map_iter->first;
				transform(name.begin(), name.end(), name.begin(), ::tolower);
				if(name == "outputdirectory") m_output_directory = map_iter->second;
				else if(name == "outputfile") m_output_file = map_iter->second;
				else if(name == "start" || name == "end"){
					//get time here
					int day, month, year, hour, minute, second;
					const char* data = map_iter->second.c_str();
					if(sscanf(data, "%d/%d/%d %d:%d:%d", &month, &day, &year, &hour, &minute, &second) != 6) break;
					struct tm to_convert;
					to_convert.tm_sec = second;
					to_convert.tm_min = minute;
					to_convert.tm_hour = hour;
					to_convert.tm_year = year;
					to_convert.tm_mday = day;
					to_convert.tm_mon = month-1;
					if(name == "start") m_start = mktime(&to_convert);
					else m_end = mktime(&to_convert);
				}
				else if(name == "user") m_user = map_iter->second;
			}
			
			time_t current_time = time(NULL);
			
			if(m_start == 0 || m_end == 0 || m_start <= current_time || m_end <= current_time || m_user == "")
				throw -1;
		} else{		
			SimulationPin* current_pin = new SimulationPin();
			int startpoint = -1, highpoint = -1, lowpoint = -1, pin = -1;
			string startpoint_response = "", high_response = "", low_response = "";
			string parameter = "", nickname = "";
			bool alarm = false;
			string alarm_response = "", alarm_arguments = "", recording_frequency = "";
			for(map<string, string>::iterator map_iter = local_settings.begin(); map_iter != local_settings.end(); map_iter++){
				string name = map_iter->first;
				transform(name.begin(), name.end(), name.begin(), ::tolower);
				if(name == "pin") pin = atoi(map_iter->second.c_str());
				else if(name == "parameter") parameter = map_iter->second;
				else if(name == "nickname") nickname = map_iter->second;
				else if(name == "startpoint") startpoint = atoi(map_iter->second.c_str());
				else if(name == "startpointfunction") startpoint_response = map_iter->second;
				else if(name == "upperlimit") highpoint = atoi(map_iter->second.c_str());
				else if(name == "upperresponse") high_response = map_iter->second;
				else if(name == "lowerlimit") lowpoint = atoi(map_iter->second.c_str());
				else if(name == "alarm"){
					if(map_iter->second[0] == 'y') alarm = true;
					else alarm = false;
				}
				else if(name == "alarmmethod") alarm_response = map_iter->second;
				else if(name == "alarmmethodarguments") alarm_arguments = map_iter->second;
				else if(name == "recordfrequency") recording_frequency = map_iter->second;
			}
			
			if(pin != -1) current_pin->setPin(pin);
			
			if(startpoint != -1 && startpoint_response != "") current_pin->setPinValues(startpoint, startpoint_response, "Startpoint");
			else{
				delete current_pin;
				continue;
			}
			
			if(highpoint != -1 && high_response != "") current_pin->setPinValues(highpoint, high_response, "Upper");
			else{
				delete current_pin;
				continue;
			}
			
			if(lowpoint != -1 && low_response != "") current_pin->setPinValues(lowpoint, low_response, "Lower");
			else{
				delete current_pin;
				continue;
			}
			
			if(recording_frequency != "") current_pin->setRecordingFrequency(recording_frequency);
			else{
				delete current_pin;
				continue;
			}
			
			if(alarm && alarm_response != ""){
				if(alarm_arguments != ""){
					alarm_response += ' ';
					alarm_response += alarm_arguments;
				}
				current_pin->setAlarm(alarm, alarm_response);
			}
			
			m_pin_data.push_back(current_pin);
		}
	}
	
	string file_name = m_output_directory + "/settings.txt";
	ofstream ostr(file_name);
	if(!ostr) throw -1;
	
	for(unsigned int i = 0; i < fileContents.size(); i++){
		ostr << fileContents[i];
	}
	
	ostr.close();
}

Simulation::~Simulation(){
	for(unsigned int i = 0; i < m_pin_data.size(); i++){
		delete m_pin_data[i];
	}
}

bool SimulationScheduler::insert(Simulation* simToInsert){
	time_t to_insert_start = simToInsert->getStartTime();
	time_t to_insert_end = simToInsert->getEndTime();
	for(vector<Simulation*>::iterator iter = m_simulations.begin(); iter != m_simulations.end(); iter++){
		time_t start_time = (*iter)->getStartTime();
		if(start_time < to_insert_start) continue;
		
		if(start_time < to_insert_end) return false;
		
		iter--;
		time_t end_time = (*iter)->getEndTime();
		if(end_time > to_insert_start) return false;
		
		iter++;
		m_simulations.insert(iter, simToInsert);
		return true;
	}
	m_simulations.push_back(simToInsert);
	return true;
}