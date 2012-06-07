#ifndef _LISTENERS_H_
#define _LISTENERS_H_

#include "MMS.h"
#include "MMSListeners.h"
#include "MMSConnections.h"

class Message{
public:
	Message();
	~Message(){}

	bool isEmpty();
	void clear();

	void setSizeOfHeader(unsigned int sizeOfHeader);

	bool addHeaderBytes(vector<byte> headerBytes);
	bool addMessageBytes(vector<byte> messageBytes);

	bool isHeaderComplete();
	bool isMessageComplete();

	int sizeRemainingForHeader();
	int sizeRemainingForMessage();

	vector<byte> getMessage();
	string getName();
	byte getTypeOfConnection();

	void setFunctionName(string functionName);
	void setParameters(vector<string> parameters);
	string getFunctionName();
	vector<string> getParameters();
private:
	vector<byte> m_header_bytes;
	vector<byte> m_message_bytes;
	unsigned int m_size_of_header;
	unsigned int m_connection_type;
	unsigned int m_size_of_message;
	string m_name;
	bool m_is_empty;
	byte m_type_of_connection;
	string m_function_name;
	vector<string> m_parameters;
};

typedef bool (*MessageHandler)(Message*, MMSConnection*);
typedef bool (*ConnectionHandler)(MMSConnection*, string);
typedef bool (*DisconnectionHandler)(MMSConnection*, string);

class MainListener : public MMSReadListener, public MMSWriteListener, public MMSConnectionListener, public MMSDisconnectionListener{
public:
	MainListener();
	~MainListener(){
		//TODO: Clean up all of the allocated memory
	}
	void initialize(string listenerName);

	bool onReadEvent(MMSReadEvent readEvent);
	bool onWriteEvent(MMSWriteEvent writeEvent);
	bool onConnectEvent(MMSConnectEvent connectEvent);
	bool onDisconnectEvent(MMSDisconnectEvent disconnectEvent);

	bool registerMessageHandler(string functionName, MessageHandler functionPointer);
	bool registerDefaultMessageHandler(MessageHandler functionPointer);

	bool registerConnectionHandler(string port, ConnectionHandler functionPointer);
	bool registerDisconnectionHandler(string port, DisconnectionHandler functionPointer);

	void handleError(SOCKET socket);

	MMSConnection* findConnection(string name);
private:
	string m_name;
	map<string, MMSConnection*> m_connections;
	map<SOCKET, MMSConnection*> m_unnamed_connections;
	map<SOCKET, Message*> m_messages;
	map<string, list<MessageHandler> > m_message_handlers;
	map<string, list<ConnectionHandler> > m_connection_handlers;
	map<string, list<DisconnectionHandler> > m_disconnection_handlers;
	MessageHandler m_default_handler;
};

#endif