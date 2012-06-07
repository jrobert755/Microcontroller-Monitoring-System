#ifndef _FILEHANDLINGGLOBAL_H_
#define _FILEHANDLINGGLOBAL_H_

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <map>
#include <list>
#include <ctime>
#include <vector>
#include <sstream>
#include <fstream>

#ifdef _WIN32
#pragma warning(disable: 4251)
#include <Windows.h>
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#define strcpy_s(x, y, z) strncpy(x, z-1, y)
#define strcat_s(x, y, z) strncpy(x, z-1, y)
#define localtime_s(x, y) localtime_r(y, x)
#endif

using std::string;
using std::map;
using std::pair;
using std::list;
using std::vector;
using std::cout;
using std::endl;
using std::ostringstream;
using std::ifstream;
using std::ofstream;
using std::ios_base;

int isDirectory(char* file_name);
int makeDirectory(char* name);

int doesFileExist(string fileName);

class File{
public:
	File(){}
	File(string full_path);
	File(const File& copy);
	~File();

	File& operator=(const File& parameter);

	string getName(){
		return m_file_name;
	}
	string getData();
	vector<string> getVectorData(){
		return m_file_lines;
	}
	void setVectorData(vector<string> data){
		m_file_lines = data;
		ofstream ostr(m_file_name.c_str(), ios_base::out | ios_base::binary);
		if(!ostr) return;
		for(vector<string>::iterator iter = data.begin(); iter != data.end(); iter++){
			if(*iter == "") continue;
			ostr << *iter;
		}
		ostr.close();
	}

	static File* createBlankFile(string name){
		File* return_file = new File();
		return_file->m_file_name = name;
		return return_file;
	}

	void reloadFile();
private:
	string m_file_name;
	vector<string> m_file_lines;
};

class Directory{
public:
	Directory(string full_path, string specific_files);
	Directory(const Directory& copy);
	~Directory();

	Directory& operator=(const Directory& parameter);

	void addDirectory(Directory* directory, string directory_name);
	void addFile(File* file, string file_name);

	string getName();

	Directory* findDirectory(string directoryName);
	File* findFile(string fileName);

	static Directory* createBlankDirectory(string name);
	void createNewFile(File* file);
	void mergeDirectory(Directory* directory, bool replace);

	map<string, File*> getAllFiles(){
		return m_files;
	}

	map<string, Directory*> getAllDirectories(){
		return m_directories;
	}
private:
	Directory();

	string m_name;
	map<string, File*> m_files;
	map<string, Directory*> m_directories;
};

#endif