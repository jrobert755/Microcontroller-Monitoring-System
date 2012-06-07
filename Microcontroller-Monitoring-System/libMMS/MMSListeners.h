#ifndef _MMSLISTENERS_H_
#define _MMSLISTENERS_H_

#include "MMS.h"

class MMSConnection;

class MMSEvent{
public:
	MMSEvent(MMSConnection* connection);

	MMSConnection* getConnection();
protected:
	MMSConnection* m_connection;
};

class MMSReadEvent : public MMSEvent{
public:
	MMSReadEvent(MMSConnection* toReadFrom) : MMSEvent(toReadFrom){}
};

class MMSWriteEvent : public MMSEvent{
public:
	MMSWriteEvent(MMSConnection* toSendTo, vector<byte> message, byte senderConnectionType, string senderName);

	vector<byte> getMessage();
	byte getSenderConnectionType();
	string getSenderName();

private:
	vector<byte> m_message;
	byte m_connection_type;
	string m_name;
};

class MMSConnectEvent : public MMSEvent{
public:
	MMSConnectEvent(MMSConnection* connection, string port) : MMSEvent(connection){
		m_port = port;
	}

	string getPort(){
		return m_port;
	}
private:
	string m_port;
};

class MMSDisconnectEvent: public MMSEvent{
public:
	MMSDisconnectEvent(MMSConnection* connection, string port) : MMSEvent(connection){
		m_port = port;
	}

	string getPort(){
		return m_port;
	}
private:
	string m_port;
};

class MMSListener{
public:
	virtual ~MMSListener(){
	}
	virtual void initialize(string listenerName) = 0;

	string getName();
protected:
	string m_name;
};

class MMSListenerComparator{
	bool operator()(MMSListener* one, MMSListener* two){
		return one->getName().compare(two->getName()) < 0;
	}
};

class MMSReadListener : virtual public MMSListener{
public:
	virtual bool onReadEvent(MMSReadEvent readEvent) = 0;
};

class MMSWriteListener : virtual public MMSListener{
public:
	virtual bool onWriteEvent(MMSWriteEvent writeEvent) = 0;
};

class MMSConnectionListener : virtual public MMSListener{
public:
	virtual bool onConnectEvent(MMSConnectEvent connectEvent) = 0;
};

class MMSDisconnectionListener : virtual public MMSListener{
public: 
	virtual bool onDisconnectEvent(MMSDisconnectEvent disconnectEvent) = 0;
};

#endif