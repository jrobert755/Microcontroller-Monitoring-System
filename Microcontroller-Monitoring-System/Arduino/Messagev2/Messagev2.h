#include <Arduino.h>

unsigned fmtUnsigned(unsigned long val, char *buf, unsigned bufLen = 0xffff, byte width = 0);
void fmtDouble(double val, byte precision, char *buf, unsigned bufLen = 0xffff);

class Message{
public:
	Message();
	void SerialRead();
	void SerialWrite(const char* message);
	bool isMessageComplete();
	const char* getMessage();
	
private:
	char* m_buffer;
	int m_current_position;
	bool m_is_complete;
};
