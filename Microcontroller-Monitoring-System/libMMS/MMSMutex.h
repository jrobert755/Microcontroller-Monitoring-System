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