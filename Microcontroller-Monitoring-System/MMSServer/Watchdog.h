#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_


#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WIN32
#else
#include <unistd.h>
#include <pthread.h>
#endif
#include <jwsmtp.h>
#include <string>

#include <MMSServer.h>

#include "Arduino.h"
#include "Handlers.h"

using std::string;

#define sleep(x) ::Sleep(x*1000)

#ifdef _WIN32
unsigned __stdcall processThread(void* arguments);
#else
void* watchdogThread(void* arguments);
#endif

#endif
