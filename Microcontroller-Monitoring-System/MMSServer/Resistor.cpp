#include "Resistor.h"

Resistor::Resistor(map<string, string> settings, Arduino* arduino){
	m_arduino = arduino;
	m_base_resistance = 0;
	m_number_of_steps = 256;
	m_max_step_number = 255;
	m_max_resistance = 100000;
	m_number_of_resistors = 6;
	m_slave_pin = 9;
	m_name = "";
	m_next_resistor_name = "";
	m_next_resistor = NULL;
	for(map<string, string>::iterator iter = settings.begin(); iter != settings.end(); iter++){
		if(iter->first == "resistanceBase"){
			m_base_resistance = atoi(iter->second.c_str());
		} else if(iter->first == "numberOfSteps"){
			m_number_of_steps = atoi(iter->second.c_str());
		} else if(iter->first == "maxStep"){
			m_max_step_number = atoi(iter->second.c_str());
		} else if(iter->first == "maxResistance"){
			m_max_resistance = atoi(iter->second.c_str());
		} else if(iter->first == "numberOfResistors"){
			m_number_of_resistors = atoi(iter->second.c_str());
		} else if(iter->first == "slavePin"){
			m_slave_pin = atoi(iter->second.c_str());
		} else if(iter->first == "resistorName"){
			m_name = iter->second;
		} else if(iter->first == "nextResistorName"){
			m_next_resistor_name = iter->second;
		} else if(iter->first == "isFirstResistor"){
			if(iter->second == "true") arduino->registerMainResistor(this);
		}
	}
	arduino->registerResistor(m_name, this);
}

void Resistor::calculateStepNeeded(double resistance, int& numberOfResistors, int** slavePin, int** resistorNumber, int** resistorStep, int currentLocation){
	double temp_resistance = resistance;

	if(m_next_resistor == NULL && m_next_resistor_name != ""){
		//INSERT CODE HERE TO GET A RESISTOR IF THERE IS SUPPOSE TO BE ANOTHER ONE
		m_next_resistor = m_arduino->getResistor(m_next_resistor_name);
	}

	if(resistance <= 0){
		int i = 0;
		for(; currentLocation < (currentLocation + m_number_of_resistors); currentLocation++){
			(*slavePin)[currentLocation] = m_slave_pin;
			(*resistorNumber)[currentLocation] = i;
			i++;
			(*resistorStep)[currentLocation] = 0;
		}
		if(m_next_resistor != NULL) m_next_resistor->calculateStepNeeded(resistance, numberOfResistors, slavePin, resistorNumber, resistorStep, currentLocation);
		return;
	}

	if(*slavePin == NULL){
		int base_resistance = getTotalBaseResistanceAlongPath();
		temp_resistance -= base_resistance;
		numberOfResistors = totalNumberOfResistorsInPath();
		*slavePin = new int[numberOfResistors];
		*resistorNumber = new int[numberOfResistors];
		*resistorStep = new int[numberOfResistors];
	}

	int current_resistor = 0;
	while(temp_resistance > 0 && current_resistor < numberOfResistors){
		int int_resistance = (int)((temp_resistance / m_max_resistance) * m_number_of_steps);
		if(int_resistance > m_max_step_number) int_resistance = m_max_step_number;
		(*slavePin)[currentLocation] = m_slave_pin;
		(*resistorNumber)[currentLocation] = current_resistor;
		(*resistorStep)[currentLocation] = int_resistance;
		temp_resistance -= (int_resistance * m_max_resistance) / m_number_of_steps;
		current_resistor++;
		currentLocation++;
	}

	if(m_next_resistor != NULL){
		m_next_resistor->calculateStepNeeded(temp_resistance, numberOfResistors, slavePin, resistorNumber, resistorStep, currentLocation);
	}
}

int Resistor::getTotalBaseResistanceAlongPath(){
	int return_int = m_base_resistance * m_number_of_resistors;

	if(m_next_resistor == NULL && m_next_resistor_name != ""){
		//INSERT CODE HERE TO GET A RESISTOR IF THERE IS SUPPOSE TO BE ANOTHER ONE
		m_next_resistor = m_arduino->getResistor(m_next_resistor_name);
	}

	if(m_next_resistor != NULL) return_int += m_next_resistor->getTotalBaseResistanceAlongPath();
	return return_int;
}

int Resistor::totalNumberOfResistorsInPath(){
	int return_int = m_number_of_resistors;

	if(m_next_resistor == NULL && m_next_resistor_name != ""){
		//INSERT CODE HERE TO GET A RESISTOR IF THERE IS SUPPOSE TO BE ANOTHER ONE
		m_next_resistor = m_arduino->getResistor(m_next_resistor_name);
	}

	if(m_next_resistor != NULL) return_int += m_next_resistor->totalNumberOfResistorsInPath();
	return return_int;
}