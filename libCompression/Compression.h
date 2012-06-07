#ifndef _COMPRESSION_H_
#define _COMPRESSION_H_

#include <string>

#include "bzip2/bzlib.h"

using std::string;

int compress(string method, char* input_data, int input_size, char* output_data, int& output_size);

#endif
