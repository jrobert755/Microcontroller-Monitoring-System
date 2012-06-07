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