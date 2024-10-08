# Shortcut for building the project using CMake
.PHONY: all clean build

all: build

run:
	./build/bin/ServerApp

build:
	mkdir -p build
	cd build && cmake .. && cmake --build .

clean:
	rm -rf build
