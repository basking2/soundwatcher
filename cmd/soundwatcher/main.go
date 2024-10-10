package main

import (
	"github.com/basking2/soundwatcher/v2/internal/ffmpeg"
)

func main() {
	fmg, err := ffmpeg.New()
	if err != nil {
		panic(err)
	}

	if err := fmg.Open("rtp://224.0.0.29:5004"); err != nil {
		panic(err)
	}

	fmg.Destroy()

}
