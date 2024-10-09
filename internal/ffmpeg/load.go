package ffmpeg

import (
	"errors"
	"fmt"
	"unsafe"
)

// #cgo pkg-config: libavutil libavformat
// #cgo LDFLAGS: -L/usr/lib -L${SRCDIR}/libs -lavformat -lavutil
// #include <libavformat/avformat.h>
// #include "util.h"
import "C"

func init() {

}

type FFMPEG struct {
	fmtctx *C.AVFormatContext
}

func errorToString(i C.int) error {
	//C.av_strerror(0, nil, 0);
	errptr := C.ffmpeg_error(0)
	err := errors.New(C.GoString(errptr))
	C.free(unsafe.Pointer(errptr))
	return err
}

func New() (*FFMPEG, error) {
	avf := C.malloc(C.sizeof_AVFormatContext)
	fmt.Printf("%v %T\n", avf, avf)
	f := &FFMPEG{
		fmtctx: (*C.AVFormatContext)(avf),
	}

	return f, nil
}

func (f *FFMPEG) Open() error {
	return nil
}

func (f *FFMPEG) Destroy() {
	if f.fmtctx != nil {
		C.free(unsafe.Pointer(f.fmtctx))
	}
}
