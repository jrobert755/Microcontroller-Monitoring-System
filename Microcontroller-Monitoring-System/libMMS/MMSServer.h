#ifndef _MMSSERVER_H_
#define _MMSSERVER_H_

#include "MMS.h"
#include "MMSConnections.h"
#include "MMSListeners.h"

#include <set>

using std::set;

class MMSServer{
public:
	MMSServer();
	~MMSServer();

	bool registerConnectionListener(MMSConnectionListener* connectionListener);
	bool registerDisconnectionListener(MMSDisconnectionListener* disconnectionListener);

	void handleConnect(MMSConnection* newConnection, string port);

	int serverLoop(string port, bool handleTimeout);

	set<string> getPorts();
	map<SOCKET, MMSConnection*>* getConnections(string port);
	
	void setRunning(bool running){
		this->running = running;
	}
private:
	map<string, map<SOCKET, MMSConnection*>*> m_connections;
	set<string> m_ports;
	MMSConnectionListener* m_connection_listener;
	MMSDisconnectionListener* m_disconnection_listener;
	bool running;
};

#endif