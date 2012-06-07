#ifndef _TAR_H_
#define _TAR_H_

#include <FileHandlingGlobal.h>

struct HeaderData{
	char name[100];
	char mode[8];                 /* 100 */
	char uid[8];                  /* 108 */
	char gid[8];                  /* 116 */
	char size[12];                /* 124 */
	char mtime[12];               /* 136 */
	char chksum[8];               /* 148 */
	char typeflag[1];             /* 156 */
	char linkname[100];           /* 157 */
	char magic[6];                /* 257 */
	char version[2];              /* 263 */
	char uname[32];               /* 265 */
	char gname[32];               /* 297 */
	char devmajor[8];             /* 329 */
	char devminor[8];             /* 337 */
	char prefix[155];			  /* 345 */
	char pad[12];				  /* 500 */
};

class TarHeader{
public:
	TarHeader();

	void initializeHeader();
	void setAttribute(int attribute);
	void setName(string file_name);
	void setSize(int size);
	void checksum();

	const char* getCharHeader();
	void unpackHeader(char* f);
	unsigned int getSize();
private:
	HeaderData header;
	static const char delim = '/';
};

class Tar{
public:
	Tar();
	~Tar();

	void addDirectory(Directory* directory);
	void addFile(File* file);

	void outputTarFile(string fileName);
	vector<char> outputTarData();
	void getData(vector<char>& data, Directory* directory, string currentName);
private:
	Directory* m_root_directory;
};

#endif