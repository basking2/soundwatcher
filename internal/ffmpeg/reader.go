package ffmpeg

import "unsafe"

// #include <stdint.h>
import "C"

//export FfmpegReader
func FfmpegReader(opaque unsafe.Pointer, buf *C.uint8_t, buf_size C.int) C.int {
	ffmpeg := (*FFMPEG)(opaque)

	if ffmpeg == nil {
		panic("NIL!?")
	}

	println(buf_size)
	return 0
}
