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

#include "Settings.h"

list<map<string, string> > SettingsLoader::loadFile(string fileName){
	list<map<string, string> > return_list;
	ifstream istr(fileName.c_str());
	if(!istr) return return_list;
	string current_line;
	string section_name;

	getline(istr, current_line);
	while(!istr.eof()){
		if(current_line == ""){
			getline(istr, current_line);
			continue;
		}
		if(current_line.find("begin") == string::npos){
			istr.close();
			return return_list;
		}

		size_t start_place = current_line.find(" ");
		if(start_place == string::npos){
			istr.close();
			return return_list;
		}
		section_name = current_line.substr(start_place + 1);
		map<string, string> insert_map;
		insert_map.insert(pair<string, string>("name", section_name));
		
		getline(istr, current_line);
		while(!istr.eof() && current_line.find("end") == string::npos){
			current_line.erase(remove_if(current_line.begin(), current_line.end(), ::isspace), current_line.end());
			string item, value;
			size_t place_of_equal_sign = current_line.find('=');
			if(place_of_equal_sign == string::npos) continue;
			item = current_line.substr(0, place_of_equal_sign);
			value = current_line.substr(place_of_equal_sign + 1);
			insert_map.insert(pair<string, string>(item, value));

			getline(istr, current_line);
		}

		/*pair<map<string, map<string, string> >::iterator, bool> iter = return_map.insert(pair<string, map<string, string> >(section_name, insert_map));
		if(!iter.second){
			map<string, string> existing_elements = iter.first->second;
			for(map<string, string>::iterator map_iter = insert_map.begin(); map_iter != insert_map.end(); map_iter++){
				existing_elements.insert(pair<string, string>(map_iter->first, map_iter->second));
			}
		}*/
		return_list.push_back(insert_map);
		getline(istr, current_line);
	}

	return return_list;
}