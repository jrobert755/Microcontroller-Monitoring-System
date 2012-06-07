/* libFileHandler
 * Copyright 2012 Jean-Luc Roberts
 *
 * This file is part of the libFileHandler package.
 *
 * libFileHandler is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public 
 * License as published by the Free Software Foundation, either 
 * version 3 of the License, or (at your option) any later version.
 *
 * libFileHandler is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with libFileHandler. If not, see 
 * <http://www.gnu.org/licenses/>.
 * 
 * A copy of the GNU Lesser General Public License can be found in
 * the parent directory for this directory.
 */

#include "FileHandlingGlobal.h"

int doesFileExist(string fileName){
#ifdef _WIN32
	wchar_t* temp = new wchar_t[256];
	MultiByteToWideChar(CP_ACP, 0, fileName.c_str(), -1, temp, 256);
	DWORD attributes = GetFileAttributes((LPCWSTR)temp);
	if(attributes == INVALID_FILE_ATTRIBUTES) return -1;
	if(attributes == FILE_ATTRIBUTE_NORMAL) return 1;
	else return 0;
#else
	struct stat buffer;
	if(lstat(fileName.c_str(), &buffer) != 0) return -1;
	if(S_ISREG(buffer.st_mode)) return 1;
	else return 0;
#endif
}

File::File(string full_path){
	m_file_name = full_path;
	ifstream istr(full_path.c_str(), ios_base::in | ios_base::binary);
	if(!istr){
		throw -1;
	}

	while(istr.good()){
		string line;
		getline(istr, line);
		if(m_file_lines.size() > 0) m_file_lines.back() += "\n";
		m_file_lines.push_back(line);
	}
	istr.close();
}

File::File(const File& copy){
	this->m_file_name = copy.m_file_name;
	this->m_file_lines = copy.m_file_lines;
}

File::~File(){
}

File& File::operator=(const File& parameter){
	this->m_file_name = parameter.m_file_name;
	this->m_file_lines = parameter.m_file_lines;
	return *this;
}

string File::getData(){
	ostringstream output_string;
	vector<string>::iterator iter = m_file_lines.begin();
	//output_string << *iter;
	for(; iter != m_file_lines.end(); iter++){
		//output_string << endl << *iter;
		output_string << *iter;
	}
	return output_string.str();
}

void File::reloadFile(){
	ifstream istr(m_file_name.c_str(), ios_base::in | ios_base::binary);
	if(!istr) return;
	m_file_lines.clear();

	while(istr.good()){
		string line;
		getline(istr, line);
		if(m_file_lines.size() > 0) m_file_lines.back() += "\n";
		m_file_lines.push_back(line);
	}
	istr.close();
}