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