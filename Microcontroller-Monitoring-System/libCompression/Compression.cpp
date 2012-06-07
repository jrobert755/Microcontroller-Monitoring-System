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

#include "Compression.h"

int compress(string method, char* input_data, int input_size, char* output_data, int& output_size){
	if(method == "bzip2"){
		bz_stream stream;
		stream.bzalloc = NULL;
		stream.bzfree = NULL;
		stream.opaque = NULL;

		int rv = BZ2_bzCompressInit(&stream, 5, 0, 0);
		if(rv != BZ_OK) return -1;
		stream.next_in = input_data;
		stream.avail_in = input_size;
		stream.next_out = output_data;
		stream.avail_out = output_size;
		rv = BZ2_bzCompress(&stream, BZ_FINISH);
		while(rv != BZ_STREAM_END){
			rv = BZ2_bzCompress(&stream, BZ_FINISH);
			if(rv != BZ_STREAM_END || rv != BZ_FINISH_OK) return -1;
		}

		output_size = stream.total_out_lo32;

		BZ2_bzCompressEnd(&stream);
		return 0;
	}
	return -1;
}