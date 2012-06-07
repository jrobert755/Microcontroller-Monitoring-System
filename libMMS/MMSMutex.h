#ifndef _MMSMUTEX_H_
#define _MMSMUTEX_H_

#include "MMS.h"

class MMSMutex{
public:
#ifdef _WIN32
	MMSMutex(){
		mutex = CreateMutex(0, FALSE, 0);
	}
	~MMSMutex(){
		CloseHandle(mutex);
	}
	bool lock(){
		DWORD rc;
		if((rc = WaitForSingleObject(mutex, INFINITE)) == WAIT_FAILED) return false;
		else return true;
	}
	void unlock(){
		ReleaseMutex(mutex);
	}
#else
	MMSMutex(){
		pthread_mutex_init(&mutex, NULL);
	}
	~MMSMutex(){
		pthread_mutex_unlock(&mutex);
		pthread_mutex_destroy(&mutex);
	}
	bool lock(){
		if(pthread_mutex_lock(&mutex) != 0) return false;
		return true;
	}
	void unlock(){
		pthread_mutex_unlock(&mutex);
	}
#endif

private:
#ifdef _WIN32
	HANDLE mutex;
#else
	pthread_mutex_t mutex;
#endif
};

#endif