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

#include "MMS.h"
#include "MMSListeners.h"
#include "MMSConnections.h"

MMSEvent::MMSEvent(MMSConnection* connection){
	m_connection = connection;
}

MMSConnection* MMSEvent::getConnection(){
	return m_connection;
}

MMSWriteEvent::MMSWriteEvent(MMSConnection* toSendTo, vector<byte> message, byte senderConnectionType, string senderName) : MMSEvent(toSendTo){
	m_message = message;
	m_connection_type = senderConnectionType;
	m_name = senderName;
}

vector<byte> MMSWriteEvent::getMessage(){
	return m_message;
}

byte MMSWriteEvent::getSenderConnectionType(){
	return m_connection_type;
}

string MMSWriteEvent::getSenderName(){
	return m_name;
}