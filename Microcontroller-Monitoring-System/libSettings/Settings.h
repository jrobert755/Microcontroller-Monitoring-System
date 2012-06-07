/* libSettings
 * Copyright 2012 Jean-Luc Roberts
 *
 * This file is part of the libSettings package.
 *
 * libSettings is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public 
 * License as published by the Free Software Foundation, either 
 * version 3 of the License, or (at your option) any later version.
 *
 * libSettings is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with libSettings. If not, see 
 * <http://www.gnu.org/licenses/>.
 * 
 * A copy of the GNU Lesser General Public License can be found in
 * the parent directory for this directory.
 */

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