
#include "clib.h"
#include "audio_output_bridge.h"
#include <stdlib.h>
#include <stdio.h>

extern "C" {
	#include <libavformat/avformat.h>
	#include <libavformat/avio.h>

	#include <libavcodec/avcodec.h>

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
	int buffersz = 4096;

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

	// FIXME - don't assign this. May be useless at this stage.
	//(*ctx)->pb = *ioctx;

	return 0;
}


int ffmpeg_read(void* opaque, AVFormatContext *ctx, AVIOContext *ioctx, const char *url) {

	AudioOutputBridge aob;
	int err = aob.init(opaque, 4096);
	if (err != 0) {
		return err;
	}

    if (avformat_open_input(&ctx, url, NULL, NULL) != 0) {
        return EXIT_FAILURE;
    }

    if (avformat_find_stream_info(ctx, NULL) < 0) {
        return EXIT_FAILURE;
    }

	int video_stream_index = 0;
	int audio_stream_index = 0;

	const AVCodec *video_codec;
	const AVCodec *audio_codec;

    video_stream_index = av_find_best_stream(ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &video_codec, 0);
    audio_stream_index = av_find_best_stream(ctx, AVMEDIA_TYPE_AUDIO, -1, -1, &audio_codec, 0);

	printf("decoder v=%d a=%d\n", av_codec_is_decoder(video_codec), av_codec_is_decoder(audio_codec));
	printf("encoder v=%d a=%d\n", av_codec_is_encoder(video_codec), av_codec_is_encoder(audio_codec));
	printf("V=%d %s \tA=%d %s\n", video_stream_index, video_codec->name, audio_stream_index, audio_codec->name);

	AVCodecContext *video_codec_context = avcodec_alloc_context3(video_codec);
	AVCodecContext *audio_codec_context = avcodec_alloc_context3(audio_codec);

	err = avcodec_open2(video_codec_context, video_codec, NULL);
	if (err != 0) {
		return err;
	}

	err = avcodec_open2(audio_codec_context, audio_codec, NULL);
	if (err != 0) {
		return err;
	}

    //start reading packets from stream and write them to file
    //err = av_read_play(ctx);    //play RTSP
	//if (err != 0) {
		// NOP - not sure if this error matters.
		//return err;
	//}

	AVPacket *packet = av_packet_alloc();
	err = av_new_packet(packet, 4096);
	if (err != 0) {
		return err;
	}

	AVPacket *out_packet = av_packet_alloc();
	//err = av_new_packet(out_packet, 4096);
	//if (err != 0) {
	//	return err;
	//}

	for (int i = 0; i < 10000; i++) {
		fflush(stdout);
		err = av_read_frame(ctx, packet);
		if (err != 0) {
			return err;
		}
		printf("Reading packet %d for stream %d size %d.\n", i, packet->stream_index, packet->size);
		printf("\t pts %d dts %d duration %d.\n", packet->pts, packet->dts, packet->duration);
		fflush(stdout);

		if (packet->stream_index == audio_stream_index) {

			err = avcodec_send_packet(audio_codec_context, packet);
			if (err != 0) {
				// FLUSH audio out and carry on. Just log the error.
				printf("EEE\n");
			    fflush(stdout);
				return err;
			}


			printf("GOT REAL DATA\n");
			fflush(stdout);
			AVFrame *frame = av_frame_alloc();
			err = avcodec_receive_frame(audio_codec_context, frame);

			if (err == AVERROR_EOF || err == AVERROR(EAGAIN)) {
				// No real error. Carry on.
			} else if (err != 0) {
				printf("EEE2\n");
			    fflush(stdout);
				return err;
			}

			//av_pkt_dump2(stdout, packet, 0, NULL);
			//fflush(stdout);
			//packet->stream_index=video_stream_index;
			//packet->pts = AV_NOPTS_VALUE;
			//packet->dts = AV_NOPTS_VALUE;
			// AVFMT_NOTIMESTAMPS
			//err = av_write_frame(aob.fmtctx, packet);
			//if (err != 0) {
			//	return err;
			//}
		}
		av_packet_unref(packet);
	}

	av_packet_free(&packet);
	//avformat_close_input(&ctx);

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
