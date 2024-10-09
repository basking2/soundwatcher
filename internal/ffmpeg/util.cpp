#include "util.h"

#include <stdlib.h>

extern "C" {
#include <libavutil/error.h>
}

char * ffmpeg_error(int i) {
	char* errstr = static_cast<char *>(malloc(AV_ERROR_MAX_STRING_SIZE));

	av_strerror(i, errstr, AV_ERROR_MAX_STRING_SIZE);

	return errstr;
}
