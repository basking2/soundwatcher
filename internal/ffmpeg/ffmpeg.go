package ffmpeg

import (
	"errors"
	"fmt"
	"unsafe"
)

// #cgo pkg-config: libavutil libavformat
// #include <libavformat/avformat.h>
// #include "clib.h"
import "C"

func init() {
}

type FFMPEG struct {
	fmtctx *C.AVFormatContext
	ioctx  *C.AVIOContext
}

// errorToString returns an error of the ffmpeg error given an error integer.
func errorToString(i C.int) error {
	errptr := C.ffmpeg_error(i)
	defer C.free(unsafe.Pointer(errptr))
	err := errors.New(C.GoString(errptr))
	return err
}

func New() (*FFMPEG, error) {

	if i := C.avformat_network_init(); i != 0 {
		return nil, errorToString(i)
	}

	f := &FFMPEG{
		fmtctx: nil,
		ioctx:  nil,
	}

	if i := C.ffmpeg_init_context(unsafe.Pointer(f), &f.fmtctx, &f.ioctx); i != 0 {
		return nil, errorToString(i)
	}

	return f, nil
}

func (f *FFMPEG) Open(url string) error {
	cstr := C.CString(url)
	defer C.free(unsafe.Pointer(cstr))

	if i := C.avformat_open_input(&f.fmtctx, cstr, nil, nil); i != 0 {
		return errorToString(i)
	}

	/*
		p := unsafe.Pointer(nil)
		avInputFormat := C.av_demuxer_iterate(&p)
		for avInputFormat != nil {
			fmt.Println(avInputFormat)
			avInputFormat = C.av_demuxer_iterate(&p)
		}
	*/

	var avInputFormat *C.AVInputFormat = nil
	fmt.Println(avInputFormat)

	C.av_probe_input_buffer2(f.ioctx, &avInputFormat, cstr, nil, 0, 0)
	fmt.Println(avInputFormat)

	if i := C.av_find_best_stream(f.fmtctx, C.AVMEDIA_TYPE_AUDIO, -1, -1, nil, 0); i == C.AVERROR_STREAM_NOT_FOUND {
		return errors.New("stream not found")
	} else if i == C.AVERROR_DECODER_NOT_FOUND {
		return errors.New("decoder not found")
	}

	if i := C.av_read_play(f.fmtctx); i != 0 {
		return errorToString(i)
	}

	return nil
}

func (f *FFMPEG) Destroy() error {
	if f.fmtctx != nil {
		C.ffmpeg_free_context(f.fmtctx, f.ioctx)
	}

	if i := C.avformat_network_deinit(); i != 0 {
		return errorToString(i)
	}

	return nil
}
