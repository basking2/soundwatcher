
.PHONY: all

default: all

clean:
	@echo none

all:
	goimports -w . && \
	go fmt ./... && \
	go build -o soundwatcher cmd/soundwatcher/main.go
