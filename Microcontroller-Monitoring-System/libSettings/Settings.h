#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <list>

using std::string;
using std::map;
using std::pair;
using std::ifstream;
using std::remove_if;
using std::list;

class SettingsLoader{
public:
	static list<map<string, string> > loadFile(string fileName);
};

#endif