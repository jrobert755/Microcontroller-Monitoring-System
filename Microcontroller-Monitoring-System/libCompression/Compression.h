/* libCompression
 * Copyright 2012 Jean-Luc Roberts
 *
 * This file is part of the libCompression package.
 *
 * libCompression is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public 
 * License as published by the Free Software Foundation, either 
 * version 3 of the License, or (at your option) any later version.
 *
 * libCompression is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with libCompression. If not, see 
 * <http://www.gnu.org/licenses/>.
 * 
 * A copy of the GNU Lesser General Public License can be found in
 * the parent directory for this directory.
 */

#ifndef _COMPRESSION_H_
#define _COMPRESSION_H_

#include <string>

#include "bzip2/bzlib.h"

using std::string;

int compress(string method, char* input_data, int input_size, char* output_data, int& output_size);

#endif
