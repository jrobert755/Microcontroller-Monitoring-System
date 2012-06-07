#include "PollCurve.h"
#include "Arduino.h"

PollCurve::PollCurve(map<string, string> settings){
	for(map<string, string>::iterator iter = settings.begin(); iter != settings.end(); iter++){
		if(iter->first == "timeToStart"){
			m_start_time = convertStringtoHMS(iter->second);
		} else if(iter->first == "resistances"){
			string current_string = "";
			for(string::iterator string_iter = iter->second.begin(); string_iter != iter->second.end(); string_iter++){
				if(*string_iter == ','){
					int value = atoi(current_string.c_str());
					if(value != 0) m_resistances.push_back(value);
					current_string = "";
				} else{
					current_string += *string_iter;
				}
			}

			if(current_string != ""){
				int value = atoi(current_string.c_str());
				if(value != 0) m_resistances.push_back(value);
			}
		} else if(iter->first == "daysToRun"){
			m_days_to_run = 0;
			string current_string = "";
			for(string::iterator string_iter = iter->second.begin(); string_iter != iter->second.end(); string_iter++){
				if(*string_iter == ','){
					if(current_string == "Sunday") m_days_to_run |= 1;
					else if(current_string == "Monday") m_days_to_run |= 2;
					else if(current_string == "Tuesday") m_days_to_run |= 4;
					else if(current_string == "Wednesday") m_days_to_run |= 8;
					else if(current_string == "Thursday") m_days_to_run |= 16;
					else if(current_string == "Friday") m_days_to_run |= 32;
					else if(current_string == "Saturday") m_days_to_run |= 64;
					else if(current_string == "Everyday"){
						m_days_to_run = -1;
						break;
					}
					current_string = "";
				} else{
					current_string += *string_iter;
				}
			}

			if(current_string != ""){
				if(current_string == "Sunday") m_days_to_run |= 1;
				else if(current_string == "Monday") m_days_to_run |= 2;
				else if(current_string == "Tuesday") m_days_to_run |= 4;
				else if(current_string == "Wednesday") m_days_to_run |= 8;
				else if(current_string == "Thursday") m_days_to_run |= 16;
				else if(current_string == "Friday") m_days_to_run |= 32;
				else if(current_string == "Saturday") m_days_to_run |= 64;
				else if(current_string == "Everyday") m_days_to_run = -1;
			}
		}
	}

	m_resistance_iter = m_resistances.begin();
}