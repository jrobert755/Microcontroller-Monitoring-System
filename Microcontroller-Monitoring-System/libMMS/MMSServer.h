/* libMMS
 * Copyright 2012 Jean-Luc Roberts
 *
 * This file is part of the libMMS package.
 *
 * libMMS is free software: you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation, either version 3 of 
 * the License, or (at your option) any later version.
 *
 * libMMS is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with libMMS. If not, see 
 * <http://www.gnu.org/licenses/>.
 * 
 * A copy of the GNU Lesser General Public License can be found in
 * the parent directory for this directory.
 */

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