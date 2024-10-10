
.PHONY: all

default: all

clean:
	@echo none

test:
	go test -timeout 3s ./...

all:
	goimports -w . && \
	go fmt ./... && \
	go build -o soundwatcher cmd/soundwatcher/main.go
