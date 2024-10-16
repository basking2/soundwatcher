
#ifndef _AUDIO_BRIDGE_H
#define _AUDIO_BRIDGE_H

#ifdef __cplusplus
extern "C" {  // only need to export C interface if
#endif

#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavutil/error.h>

/**
 * Wrap creation and destruction of output resources.
 */
struct AudioOutputBridge {
	AVFormatContext* fmtctx;
	AVIOContext* ioctx;

	AudioOutputBridge();
	~AudioOutputBridge();

	/**
	 * Actually do the initialization.
	 */
	int init(void* opaque, int buffersz);
};

#ifdef __cplusplus
}
#endif

#endif