#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#include <MMS.h>
#include <MMSConnections.h>
#include <Listeners.h>
#include <FileHandlingGlobal.h>
#include <ctime>
#include <queue>

#include "PollCurve.h"
//#include "Resistor.h"
class Resistor;

using std::queue;

#if defined(__linux__)
#define sscanf_s sscanf
#endif

string convertHMStoString(time_t timeToConvert);
time_t convertStringtoHMS(string timeToConvert);

string convertMDYtoString(time_t timeToConvert);
time_t convertStringtoMDY(string timeToConvert);

class ArduinoPinData{
public:
	bool addNewReading(int pinNumber, time_t timeOfReading, double reading);
	map<time_t, double> getReadings(int pinNumber, time_t startTime, time_t endTime, time_t interval);
	vector<string> getAllReadingsByPin();
	void addAllReadings(map<string, File*> files);
private:
	map<int, map<time_t, double> > m_pin_readings;
};

class Arduino{
public:
	Arduino();
	Arduino(MMSConnection* connection);

	bool setName(string name);

	bool addNewReading(int pinNumber, time_t timeOfReading, double reading);
	bool addNewTemperatureReading(double temperatureReading);
	map<time_t, double> getReadings(int pinNumber, time_t startTime, time_t endTime, time_t interval);
	vector<string> getAllReadingsByPin();

	MMSConnection* getConnection();
	void setConnection(MMSConnection* connection);

	bool addNewMessage(vector<byte> messageToSend, byte senderConnectionType, string sender);
	bool writeNextMessage();
	bool resendLastMessage();

	int getNextPinReadingTime();
	int getKeepAliveTime();
	int getPinsToRead();
	void handledMessage();
	PollCurve* getPollCurveData();
	void startGettingPollCurveData(){
		m_are_getting_poll_curve = true;
	}
	bool gettingPollCurveData(){
		return m_are_getting_poll_curve;
	}
	void stopGettingPollCurveData(){
		m_are_getting_poll_curve = false;
	}

	void resetConnection(MMSConnection* connection);

	void registerResistor(string name, Resistor* resistor);

	void registerMainResistor(Resistor* resistor){
		m_main_resistor = resistor;
	}

	Resistor* getMainResistor(){
		return m_main_resistor;
	}

	Resistor* getResistor(string name){
		map<string, Resistor*>::iterator iter = m_resistors.find(name);
		if(iter == m_resistors.end()) return NULL;
		else return iter->second;
	}

	bool isConnected(){
		return m_is_connected;
	}
	void setConnected(bool connected){
		m_is_connected = connected;
	}

	void reloadSettings();
private:
	bool addReadingToFile(File* file, int pinNumber, time_t timeOfReading, double reading);
	string m_name;
	ArduinoPinData* m_data;
	MMSConnection* m_connection;
	Directory* m_directory;
	bool m_is_connected;

	int m_nextPinReading;
	int m_keepAlive;
	int m_pinsToRead;

	bool m_processing;
	queue<vector<byte> > m_message_queue_message;
	queue<byte> m_message_queue_sender_type;
	queue<string> m_message_queue_sender;

	vector<byte> m_last_message_sent_message;
	byte m_last_message_sent_type;
	string m_last_message_sent_sender;

	PollCurve* m_poll_curve;
	bool m_are_getting_poll_curve;

	map<string, Resistor*> m_resistors;
	Resistor* m_main_resistor;
};

#endif