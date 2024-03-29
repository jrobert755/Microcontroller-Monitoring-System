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

#ifndef _MMS_H_
#define _MMS_H_

#if defined(_WIN32)
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#define CloseConnection closesocket
#elif defined(__linux__)
#include <pthread.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
//typedef unsigned int SOCKET;
typedef int SOCKET;
typedef unsigned char byte;
#define CloseConnection close
#else
#endif

#include <vector>
#include <string>
#include <set>
#include <map>
#include <list>
#include <sstream>
#include <utility>

using std::vector;
using std::string;
using std::set;
using std::map;
using std::pair;
using std::list;
using std::ostringstream;

void *get_in_addr(struct sockaddr *sa);

template<class T, class N>
string convertMapToString(map<T, N> toConvert){
	if(toConvert.size() == 0) return "";
	ostringstream output_stream;
	typename map<T, N>::iterator iter = toConvert.begin();
	output_stream << iter->first << ":" << iter->second;
	iter++;
	for(; iter != toConvert.end(); iter++){
		output_stream << " " << iter->first << ":" << iter->second;
	}
	return output_stream.str();
}

#endif