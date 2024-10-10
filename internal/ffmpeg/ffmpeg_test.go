package ffmpeg

import "testing"

func TestOpen(t *testing.T) {
	fmg, err := New()
	if err != nil {
		panic(err)
	}

	if err := fmg.Open("rtp://224.0.0.29:5004"); err != nil {
		panic(err)
	}

	fmg.Destroy()
}
