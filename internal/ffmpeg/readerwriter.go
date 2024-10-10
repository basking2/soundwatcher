package ffmpeg

import (
	"fmt"
	"unsafe"
)

// #include <stdint.h>
import "C"

//export FfmpegWriter
func FfmpegWriter(opaque unsafe.Pointer, buf *C.uint8_t, buf_size C.int) C.int {
	ffmpeg := (*FFMPEG)(opaque)

	if ffmpeg == nil {
		return -1
	}

	gobuf := C.GoBytes(unsafe.Pointer(buf), buf_size)

	fmt.Println(buf_size, gobuf[0:20])
	return buf_size
}

//export FfmpegReader
func FfmpegReader(opaque unsafe.Pointer, buf *C.uint8_t, buf_size C.int) C.int {
	ffmpeg := (*FFMPEG)(opaque)

	if ffmpeg == nil {
		return -1
	}

	gobuf := C.GoBytes(unsafe.Pointer(buf), buf_size)

	fmt.Println(buf_size, gobuf[0:20])
	return buf_size
}
