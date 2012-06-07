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