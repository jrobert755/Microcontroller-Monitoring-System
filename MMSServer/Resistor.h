#ifndef _RESISTOR_H_
#define _RESISTOR_H_
#include <map>
#include <string>
#include "Arduino.h"

using std::map;
using std::string;

class Resistor{
public:
	Resistor(map<string, string> settings, Arduino* arduino);

	string getName();
	string getNextResistorName();
	void calculateStepNeeded(double resistance, int& numberOfResistors, int** slavePin, int** resistorNumber, int** resistorStep, int currentLocation = 0);
	//int getSlavePin();

	int getTotalBaseResistanceAlongPath();
	int totalNumberOfResistorsInPath();
private:
	string m_name;
	string m_next_resistor_name;
	int m_max_resistance;
	int m_number_of_steps;
	int m_max_step_number;
	int m_base_resistance;
	int m_slave_pin;
	int m_number_of_resistors;
	Arduino* m_arduino;
	Resistor* m_next_resistor;
};

#endif