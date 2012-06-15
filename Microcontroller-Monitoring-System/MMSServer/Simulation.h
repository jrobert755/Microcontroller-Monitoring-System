#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <ctime>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>

using std::string;
using std::list;
using std::map;
using std::vector;
using std::transform;

using std::ofstream;

class SimulationPin{
public:
	SimulationPin(){}
	void setPinValues(int value, string response, string valueType);
	void setPin(int pin);
	void setParameter(string parameter);
	void setNickname(string nickname);
	void setAlarm(bool alarm, string alarmResponse);
	void setRecordingFrequency(string recordingFrequency);
private:
	int m_high_value, m_low_value;
	string m_high_response, m_low_response;
	int m_pin;
	string m_parameter;
	string m_nickname;
	int m_startpoint;
	string m_startpoint_response;
	bool m_alarm;
	string m_alarm_response;
	string m_record_frequency;
};


typedef unsigned char byte;

class Simulation{
public:
	Simulation(list<map<string, string> > settings, vector<byte> fileContents);
	~Simulation();
	
	time_t getStartTime(){ return m_start; }
	time_t getEndTime(){ return m_end; }
private:
	vector<SimulationPin*> m_pin_data;
	string m_output_directory;
	string m_output_file;
	time_t m_start, m_end;
	string m_user;
};

class SimulationScheduler{
public:
	bool insert(Simulation* simToInsert);
private:
	vector<Simulation*> m_simulations;
};

#endif