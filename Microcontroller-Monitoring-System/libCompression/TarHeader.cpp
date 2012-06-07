#include "Tar.h"

TarHeader::TarHeader(){
}

void TarHeader::initializeHeader(){
	memset(&header, 0, sizeof(TarHeader));
	sprintf(header.magic,"ustar");
	header.magic[5] = '\0';
	strcpy(header.version, "00");
	sprintf(header.mtime,"%011lo",time(NULL));
	sprintf(header.mode,"%06o",0777);
	sprintf(header.uid, "%07o", 0);
	sprintf(header.gid, "%07o", 0);
	sprintf(header.gname,"%s","users");
}

void TarHeader::setAttribute(int attribute){
	sprintf(header.typeflag, "%d", attribute);
}

void TarHeader::setName(string file_name){
	const char* c_name = file_name.c_str();
	strcpy(header.name, c_name);
	int length = strlen(c_name);
	if(file_name[length-1] != delim && header.typeflag[0] == '5') header.name[length] = delim;
}

void TarHeader::setSize(int size){
	sprintf(header.size, "%011llo", (long long unsigned int)size);
}

void TarHeader::checksum(){
	unsigned int sum = 0;
	char* p = (char *) &header;
	char* q = p + sizeof(HeaderData);
    while(p < header.chksum) sum += *p++ & 0xff;
	for(int i = 0; i < 8; ++i){
		sum += ' ';
		++p;
	}
	while(p < q) sum += *p++ & 0xff;

	sprintf(header.chksum,"%06o",sum);
}

void TarHeader::unpackHeader(char* f){
	memcpy(header.name, f, 100);
	memcpy(header.mode, &f[100], 8);
	memcpy(header.uid, &f[108], 8);
	memcpy(header.gid, &f[116], 8);
	memcpy(header.size, &f[124], 12);
	memcpy(header.mtime, &f[136], 12);
	memcpy(header.chksum, &f[148], 8);
	memcpy(header.typeflag, &f[156], 1);
	memcpy(header.linkname, &f[157], 100);
	memcpy(header.magic, &f[257], 6);
	memcpy(header.version, &f[263], 2);
	memcpy(header.uname, &f[265], 32);
	memcpy(header.gname, &f[297], 32);
	memcpy(header.devmajor, &f[329], 8);
	memcpy(header.devminor, &f[337], 8);
	memcpy(header.prefix, &f[345], 155);
}

unsigned int TarHeader::getSize(){
	unsigned int ret;
	sscanf(header.size, "%o", &ret);
	return ret;
}

const char* TarHeader::getCharHeader(){
	return (char*)&header;
}