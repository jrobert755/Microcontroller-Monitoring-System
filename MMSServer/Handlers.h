#ifndef _HANDLERS_H_
#define _HANDLERS_H_

#include <MMS.h>
#include <MMSConnections.h>
#include <MMSListeners.h>
#include <Listeners.h>
#include <MMSServer.h>
#include <Tar.h>
#include <Compression.h>
#include "Arduino.h"

extern map<string, Arduino*> known_arduinos;
extern map<SOCKET, MMSConnection*>* computers;
extern MMSServer* server;

bool handleArduinoConnection(MMSConnection* connection, string port);
bool handleArduinoDisconnection(MMSConnection* connection, string port);
bool authorize(Message* message, MMSConnection* connection);
bool hello(Message* message, MMSConnection* connection);
bool ok(Message* message, MMSConnection* connection);
bool addPinData(Message* message, MMSConnection* connection);
bool resendLastMessage(Message* message, MMSConnection* connection);

bool handleComputerConnection(MMSConnection* connection, string port);
bool getPinData(Message* message, MMSConnection* connection);
bool getConnectedArduinos(Message* message, MMSConnection* connection);
bool getLogs(Message* message, MMSConnection* connection);
bool getLogsByPin(Message* message, MMSConnection* connection);
bool setArduinoDigitalPin(Message* message, MMSConnection* connection);

#endif