package main

import (
	"fmt"

	"github.com/basking2/soundwatcher/v2/internal/ffmpeg"
)

func main() {
	fmg, _ := ffmpeg.New()
	fmt.Printf("Main %v!\n", fmg)

	fmg.Destroy()

}
