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

int isDirectory(char* file_name){
#ifdef _WIN32
	wchar_t* temp = new wchar_t[256];
	MultiByteToWideChar(CP_ACP, 0, file_name, -1, temp, 256);
	DWORD attributes = GetFileAttributes((LPCWSTR)temp);
	if(attributes == INVALID_FILE_ATTRIBUTES) return -1;
	if(attributes == FILE_ATTRIBUTE_DIRECTORY) return 1;
	else return 0;
#else
	struct stat buffer;
	if(lstat(file_name, &buffer) != 0) return -1;
	if(S_ISDIR(buffer.st_mode)) return 1;
	else return 0;
#endif
}

int makeDirectory(char* name){
#ifdef _WIN32
	return _mkdir(name);
#else
	return mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

Directory::Directory(){
	m_name = "";
}

Directory::Directory(string& full_path, string specific_files){
#ifdef _WIN32
	char* full_path_name = new char[MAX_PATH];
	strcpy_s(full_path_name, MAX_PATH, full_path.c_str());
	if(full_path_name[strlen(full_path_name)-1] != '/'){
		strcat_s(full_path_name, MAX_PATH, "/");
	}
	strcat_s(full_path_name, MAX_PATH, "*");
	wchar_t* wide_full_path_name = new wchar_t[256];
	MultiByteToWideChar(CP_ACP, 0, full_path_name, -1, wide_full_path_name, 256);
	WIN32_FIND_DATA find_data;
	HANDLE handle = FindFirstFile((LPCWSTR)wide_full_path_name, &find_data);
	if(handle == INVALID_HANDLE_VALUE) throw -1;
	do
	{
		char* file_name = new char[256];
		WideCharToMultiByte(CP_ACP, 0, find_data.cFileName, -1, file_name, 256, NULL, NULL);
		if(strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0){
			delete[] file_name;
			continue;
		}
		if(full_path_name[0] != '.'){
			char* temp = new char[256];
			strcpy_s(temp, 256, full_path.c_str());
			if(temp[strlen(temp)-1] != '/'){
				strcat_s(temp, 256, "/");
			}
			strcat_s(temp, 256, file_name);
			strcpy_s(file_name, 256, temp);
			delete[] temp;
		}
		int is_directory = isDirectory(file_name);
		if(strcmp(specific_files.c_str(), "") != 0 && strstr(file_name, specific_files.c_str()) == NULL && is_directory != 1){
			delete[] file_name;
			continue;
		}

		string str_file_name = file_name;
		if(is_directory == 1){
			try{
				Directory* new_directory = new Directory(str_file_name, specific_files);
				m_directories.insert(pair<string, Directory*>(str_file_name, new_directory));
			} catch(int e){
				cout << "Error creating new directory: " << e << endl;
			}
		} else{
			try{
				File* new_file = new File(str_file_name);
				size_t position = str_file_name.find_last_of('/');
				string short_name;
				if(position == string::npos) short_name = str_file_name;
				else short_name = str_file_name.substr(position + 1);
				//m_files.insert(pair<string, File*>(str_file_name, new_file));
				m_files.insert(pair<string, File*>(short_name, new_file));
			} catch(int e){
				cout << "Error creating new file: " << str_file_name << endl;
			}
		}
		delete[] file_name;
	}
	while(FindNextFile(handle, &find_data) != 0);

	FindClose(handle);
	delete[] full_path_name;
	delete[] wide_full_path_name;
	m_name = full_path;
#else
	//#error Linux code not supported
	char* full_path_name = new char[512];
	/*strcpy_s(full_path_name, 511, full_path.c_str());
	if(full_path_name[strlen(full_path_name)-1] != '/'){
		strcat_s(full_path_name, 511, "/");
	}*/
	strcpy(full_path_name, full_path.c_str());
	if(full_path_name[strlen(full_path_name)-1] != '/'){
		strcat(full_path_name, "/");
	}
	DIR* directory_info = opendir(full_path_name);
	struct dirent* info;
	while((info = readdir(directory_info)) != NULL){
		char* file_name = new char[256];
		strcpy(file_name, info->d_name);
		if(strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0){
			delete[] file_name;
			continue;
		}
		if(full_path_name[0] != '.'){
			char* temp = new char[256];
			strcpy(temp, full_path.c_str());
			if(temp[strlen(temp)-1] != '/'){
				strcat(temp, "/");
			}
			strcat(temp, file_name);
			strcpy(file_name, temp);
			delete[] temp;
		}
		int is_directory = isDirectory(file_name);
		if(strcmp(specific_files.c_str(), "") != 0 && strstr(file_name, specific_files.c_str()) == NULL && is_directory != 1){
			delete[] file_name;
			continue;
		}

		string str_file_name = file_name;
		if(is_directory == 1){
			try{
				Directory* new_directory = new Directory(str_file_name, specific_files);
				m_directories.insert(pair<string, Directory*>(str_file_name, new_directory));
			} catch(int e){
				cout << "Error creating new directory: " << e << endl;
			}
		} else{
			try{
				File* new_file = new File(str_file_name);
				size_t position = str_file_name.find_last_of('/');
				string short_name;
				if(position == string::npos) short_name = str_file_name;
				else short_name = str_file_name.substr(position + 1);
				//m_files.insert(pair<string, File*>(str_file_name, new_file));
				m_files.insert(pair<string, File*>(short_name, new_file));
			} catch(int e){
				cout << "Error creating new file: " << str_file_name << endl;
			}
		}
		delete[] file_name;
	}

	delete[] full_path_name;
	m_name = full_path;
#endif
}

Directory::Directory(const Directory& copy){
	this->m_name = copy.m_name;
	for(map<string, Directory*>::const_iterator iter = copy.m_directories.begin(); iter != copy.m_directories.end(); iter++){
		string directory_name = iter->first;
		Directory* to_insert = new Directory(*(iter->second));
		m_directories.insert(pair<string, Directory*>(directory_name, to_insert));
	}
	for(map<string, File*>::const_iterator iter = copy.m_files.begin(); iter != copy.m_files.end(); iter++){
		string file_name = iter->first;
		File* to_insert = new File(*(iter->second));
		m_files.insert(pair<string, File*>(file_name, to_insert));
	}
}

Directory::~Directory(){
	for(map<string, Directory*>::iterator iter = m_directories.begin(); iter != m_directories.end(); iter++){
		delete iter->second;
	}
	for(map<string, File*>::iterator iter = m_files.begin(); iter != m_files.end(); iter++){
		delete iter->second;
	}
}

Directory& Directory::operator=(const Directory& parameter){
	this->m_name = parameter.m_name;
	this->m_directories.clear();
	this->m_files.clear();
	for(map<string, Directory*>::const_iterator iter = parameter.m_directories.begin(); iter != parameter.m_directories.end(); iter++){
		string directory_name = iter->first;
		Directory* to_insert = new Directory(*(iter->second));
		m_directories.insert(pair<string, Directory*>(directory_name, to_insert));
	}
	for(map<string, File*>::const_iterator iter = parameter.m_files.begin(); iter != parameter.m_files.end(); iter++){
		string file_name = iter->first;
		File* to_insert = new File(*(iter->second));
		m_files.insert(pair<string, File*>(file_name, to_insert));
	}
	return *this;
}

void Directory::addDirectory(Directory* directory, string directory_name){
	const char* c_directory_name = directory_name.c_str();
	const char* location = strchr(c_directory_name, '/');
	if(location != NULL){
		//int span = strcspn(c_directory_name, "/");
		size_t span = strcspn(c_directory_name, "/");
		char* name = new char[span + 1];
		strcpy_s(name, span, c_directory_name);
		name[span] = '\0';
		string new_directory_name = name;
		Directory* new_directory;
		map<string, Directory*>::iterator iter = m_directories.find(new_directory_name);
		if(iter != m_directories.end()){
			new_directory = iter->second;
		} else {
			new_directory = Directory::createBlankDirectory(new_directory_name);
			m_directories.insert(pair<string, Directory*>(new_directory_name, new_directory));
		}
		string sub_name = (location + 1);
		new_directory->addDirectory(directory, sub_name);
	} else{
		map<string, Directory*>::iterator return_val = m_directories.find(directory_name);
		if(return_val == m_directories.end()){
			Directory* new_directory = new Directory(*directory);
			m_directories.insert(pair<string, Directory*>(directory_name, new_directory));
		} else{
			return_val->second->mergeDirectory(directory, true);
		}
	}
}

void Directory::addFile(File* file, string file_name){
	const char* c_file_name = file_name.c_str();
	const char* location = strchr(c_file_name, '/');
	if(location != NULL){
		//int span = strcspn(c_file_name, "/");
		size_t span = strcspn(c_file_name, "/");
		char* name = new char[span + 1];
		strcpy_s(name, span, c_file_name);
		name[span] = '\0';
		string new_directory_name = name;
		Directory* new_directory;
		map<string, Directory*>::iterator iter = m_directories.find(new_directory_name);
		if(iter != m_directories.end()){
			new_directory = iter->second;
		} else {
			new_directory = Directory::createBlankDirectory(new_directory_name);
			m_directories.insert(pair<string, Directory*>(new_directory_name, new_directory));
		}
		string sub_name = (location + 1);
		new_directory->addFile(file, sub_name);
	} else{
		map<string, File*>::iterator iter = m_files.find(file_name);
		File* new_file = new File(*file);
		if(iter == m_files.end()){
			m_files.insert(pair<string, File*>(file_name, new_file));
		} else{
			iter->second = new_file;
		}
	}
}

string Directory::getName(){
	return m_name;
}

File* Directory::findFile(string fileName){
	const char* c_file_name = fileName.c_str();
	const char* location = strchr(c_file_name, '/');
	if(location != NULL){
		//int span = strcspn(c_file_name, "/");
		size_t span = strcspn(c_file_name, "/");
		char* name = new char[span + 1];
		strcpy_s(name, span, c_file_name);
		name[span] = '\0';
		string new_directory_name = name;
		map<string, Directory*>::iterator iter = m_directories.find(new_directory_name);
		if(iter != m_directories.end()){
			string sub_name = (location + 1);
			return iter->second->findFile(sub_name);
		} else {
			return NULL;
		}
	} else{
		map<string, File*>::iterator iter = m_files.find(fileName);
		if(iter == m_files.end()) return NULL;
		else return iter->second;
	}
}

Directory* Directory::createBlankDirectory(string name){
	Directory* return_directory = new Directory();
	return_directory->m_name = name;
	return return_directory;
}

void Directory::createNewFile(File* file){
	if(doesFileExist(file->getName())) return;
	ofstream ostr(file->getName().c_str());
	if(!ostr) return;
	ostr << file->getData();
	ostr.close();
}

void Directory::mergeDirectory(Directory* directory, bool replace){
	map<string, Directory*> to_merge_directories = directory->m_directories;
	for(map<string, Directory*>::iterator iter = to_merge_directories.begin(); iter != to_merge_directories.end(); iter++){
		//pair<map<string, Directory*>::iterator, bool> return_value = this->m_directories.insert(pair<string, Directory*>(iter->first, iter->second));
		map<string, Directory*>::iterator found = this->m_directories.find(iter->first);
		if(found == this->m_directories.end()){
			Directory* to_insert = new Directory(*(iter->second));
			this->m_directories.insert(pair<string, Directory*>(iter->first, to_insert));
		} else {
			found->second->mergeDirectory(iter->second, replace);
		}
	}

	map<string, File*> to_merge_files = directory->m_files;
	for(map<string, File*>::iterator iter = to_merge_files.begin(); iter != to_merge_files.end(); iter++){
		map<string, File*>::iterator found = this->m_files.find(iter->first);
		File* to_insert = new File(*(iter->second));
		if(found == this->m_files.end()){
			m_files.insert(pair<string, File*>(iter->first, to_insert));
		} else if(replace) {
			delete found->second;
			found->second = to_insert;
		}
	}
}
