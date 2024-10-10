
#ifndef _GOFFMPEG_H
#define _GOFFMPEG_H

#ifdef __cplusplus
extern "C" {  // only need to export C interface if
#endif

#include <libavformat/avio.h>
#include <libavformat/avformat.h>

char * ffmpeg_error(int i);
int ffmpeg_init_context(void* opque, AVFormatContext **ctx, AVIOContext **ioctx);
int ffmpeg_free_context(AVFormatContext *ctx, AVIOContext *ioctx);

#ifdef __cplusplus
}
#endif

#endif