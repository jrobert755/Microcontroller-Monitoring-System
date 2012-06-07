#ifndef _MMSCONNECTIONS_H_
#define _MMSCONNECTIONS_H_

#include "MMS.h"
#include "MMSMutex.h"
#include "MMSListeners.h"
#include <ctime>
#include <cstring>
#ifndef _WIN32
#include <fcntl.h>
#endif

//MMS - Micro-controller Monitoring System
class MMSConnection{
public:
	MMSConnection();
	MMSConnection(SOCKET socket, sockaddr_storage& their_addr);
	~MMSConnection();

	int connectToHost(string host, string port, bool bindingMode);
	int beginListening(string port, bool bindingMode);

	SOCKET getSocket();

	//Returns 0 on success, -1 on error
	int registerListener(string listenerType, MMSListener* listener);

	bool handleRead();
	bool handleWrite(vector<byte> messageToSend, byte senderConnectionType, string senderName);

	string getAddress();

	bool isConnected();
	void setConnected(bool connected);

	//SHOULD ONLY BE USED INSIDE READ AND WRITE LISTENERS
	vector<byte> mms_read(int bytesToRead, int& error);
	bool mms_write(vector<byte> messageToSend);

	time_t getLastHeardFrom();
private:
	SOCKET m_socket;
	bool m_connected;
	MMSMutex* m_mutex;

	string m_address;

	MMSReadListener* m_read_listener;
	MMSWriteListener* m_write_listener;
	MMSDisconnectionListener* m_disconnect_listener;
	time_t m_last_heard_from;
};

#endif