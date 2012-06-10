#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <jwsmtp.h>
#include <string>

#include <MMSServer.h>

#include "Arduino.h"
#include "Handlers.h"

using std::string;

void* watchdogThread(void* arguments);

#endif
