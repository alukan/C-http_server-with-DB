# Shortcut for building the project using CMake
.PHONY: all clean build

all: build

build:
	mkdir -p build
	cd build && cmake .. && cmake --build .

clean:
	rm -rf build
