#ifndef _POLLCURVE_H_
#define _POLLCURVE_H_

#include <list>
#include <map>
#include <string>

using std::list;
using std::map;
using std::string;

class PollCurve{
public:
	PollCurve(map<string, string> settings);

	void resetPollCurve(){
		m_resistance_iter = m_resistances.begin();
	}
	int getNextResistance(){
		int return_int;
		if(m_resistance_iter != m_resistances.end()) return_int = *m_resistance_iter;
		else return_int = -1;
		m_resistance_iter++;
		return return_int;
	}
	time_t getStartTime(){
		return m_start_time;
	}
	unsigned char getDaysToRun(){
		return m_days_to_run;
	}
private:
	list<int> m_resistances;
	list<int>::iterator m_resistance_iter;
	time_t m_start_time;
	unsigned char m_days_to_run;
};

#endif