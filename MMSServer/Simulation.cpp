#include "Simulation.h"

SimulationPin::SimulationPin(){
}

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
}