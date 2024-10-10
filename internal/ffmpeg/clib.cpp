
#include "clib.h"
#include <stdlib.h>

extern "C" {
	#include <libavformat/avformat.h>
	#include <libavformat/avio.h>
	#include <libavutil/error.h>
	#include <libavutil/attributes.h>
	#include <libavutil/dict.h>
	#include <libavutil/log.h>
	#include <libavutil/mem.h>

	#include "_cgo_export.h"
}

char * ffmpeg_error(int i) {
	char* errstr = static_cast<char *>(malloc(AV_ERROR_MAX_STRING_SIZE));

	av_strerror(i, errstr, AV_ERROR_MAX_STRING_SIZE);

	return errstr;
}

int ffmpeg_init_context(void* opaque, AVFormatContext **ctx, AVIOContext **ioctx) {
	*ctx = NULL;
	int buffersz = 1024000;

	*ioctx = avio_alloc_context(
		(unsigned char*)av_malloc(buffersz),
		buffersz,
		1,
		opaque, // USER DATA GOES HERE
		NULL, // read from user
		&FfmpegWriter, // write to user
		NULL // seek
	);

	*ctx = avformat_alloc_context();
	if (ctx == NULL) {
		return AVERROR(ENOMEM);
	}

	(*ctx)->pb = *ioctx;

	return 0;
}

int ffmpeg_free_context(AVFormatContext *ctx, AVIOContext *ioctx) {
	if (ctx != NULL) {
		avformat_free_context(ctx);
	}

	if (ioctx != NULL) {
		av_free(ioctx->buffer);
		avio_context_free(&ioctx);
	}
	return 0;
}
