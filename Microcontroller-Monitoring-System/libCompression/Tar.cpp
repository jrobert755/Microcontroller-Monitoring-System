/* libCompression
 * Copyright 2012 Jean-Luc Roberts
 *
 * This file is part of the libCompression package.
 *
 * libCompression is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public 
 * License as published by the Free Software Foundation, either 
 * version 3 of the License, or (at your option) any later version.
 *
 * libCompression is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with libCompression. If not, see 
 * <http://www.gnu.org/licenses/>.
 * 
 * A copy of the GNU Lesser General Public License can be found in
 * the parent directory for this directory.
 */

#include "Tar.h"

#include <sstream>

using std::ostringstream;

Tar::Tar(){
	m_root_directory = Directory::createBlankDirectory(".");
}

Tar::~Tar(){
	delete m_root_directory;
}

void Tar::addDirectory(Directory* directory){
	if(directory->getName() == "."){
		m_root_directory->mergeDirectory(directory, true);
	} else{
		m_root_directory->addDirectory(directory, directory->getName());
	}
}

void Tar::addFile(File* file){
	m_root_directory->addFile(file, file->getName());
}

vector<char> Tar::outputTarData(){
	vector<char> return_value;
	getData(return_value, m_root_directory, "");
	for(int i = 0; i < 1024; i++){
		return_value.push_back(0);
	}
	return return_value;
}

void Tar::getData(vector<char>& data, Directory* directory, string currentName){
	map<string, File*> files = directory->getAllFiles();
	for(map<string, File*>::iterator iter = files.begin(); iter != files.end(); iter++){
		File* current_file = iter->second;
		TarHeader header;
		header.initializeHeader();
		header.setAttribute(0);
		if(currentName == "") header.setName(iter->first);
		else header.setName(currentName + "/" + iter->first);

		vector<string> lines = current_file->getVectorData();
		ostringstream ostr;
		//if(lines.size() > 0) ostr << lines[0];
		//for(int i = 1; i < lines.size(); i++){
			//ostr << endl << lines[i];
		for(unsigned int i = 0; i < lines.size(); i++){
			ostr << lines[i];
		}

		string file_contents = ostr.str();
		int file_size = file_contents.size();
		header.setSize(file_size);
		header.checksum();

		const char* char_header = header.getCharHeader();
		for(int i = 0; i < 512; i++){
			data.push_back(char_header[i]);
		}

		for(int i = 0; i < file_size; i++){
			data.push_back(file_contents[i]);
		}

		while((data.size() % 512) != 0) data.push_back(0);
	}

	map<string, Directory*> directories = directory->getAllDirectories();
	for(map<string, Directory*>::iterator iter = directories.begin(); iter != directories.end(); iter++){
		TarHeader header;
		header.initializeHeader();
		header.setAttribute(5);
		header.setSize(0);
		string name;
		//if(currentName == ""){
			name = iter->first;
		//}
		//else{
		//	name = currentName + "/" + iter->first;
		//}
		header.setName(name);
		header.checksum();
		const char* char_header = header.getCharHeader();
		for(int i = 0; i < 512; i++){
			data.push_back(char_header[i]);
		}
		getData(data, iter->second, name);
	}
}