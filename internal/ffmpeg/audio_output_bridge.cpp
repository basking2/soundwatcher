#include "audio_output_bridge.h"
#include <stdlib.h>
#include <stdio.h>

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

/**
 * Wrap creation and destruction of output resources.
 */
AudioOutputBridge::AudioOutputBridge() {
}

AudioOutputBridge::~AudioOutputBridge() {
	if (fmtctx != NULL) {
		avformat_free_context(fmtctx);
	}

	if (ioctx != NULL) {
		av_free(ioctx->buffer);
		avio_context_free(&ioctx);
	}
}

/**
 * Actually do the initialization.
 */
int AudioOutputBridge::init(void* opaque, int buffersz) {
	ioctx = avio_alloc_context(
		(unsigned char*)av_malloc(buffersz),
		buffersz,
		1,
		opaque, // USER DATA GOES HERE
		NULL, // read from user
		&FfmpegWriter, // write to user
		NULL // seek
	);

	const AVOutputFormat* oformat = NULL;
	void* p = NULL;
	for (oformat = av_muxer_iterate(&p); oformat != NULL; oformat = av_muxer_iterate(&p)) {
		if (strcmp(oformat->name, "s32be")) {
			break;
		}
		//printf("MUXER %s - %s\n", oformat->name, oformat->long_name);
		//fflush(stdout);
	}

	if (oformat == NULL) {
		return AVERROR_MUXER_NOT_FOUND;
	}

	fmtctx = avformat_alloc_context();
	if (fmtctx == NULL) {
		return AVERROR(ENOMEM);
	}

	fmtctx->pb = ioctx;
	fmtctx->oformat = oformat;

	// Cleaned up by avformat_free_context().
	AVStream *stream = avformat_new_stream(fmtctx, NULL);

	int err = avformat_init_output(fmtctx, NULL);
	if (err < 0) {
		return err;
	}

/*
	err = avformat_write_header(fmtctx, NULL);
	if (err < 0) {
		return err;
	}

*/
	return 0;
}
