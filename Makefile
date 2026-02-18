VCPKG_ROOT ?= $(HOME)/vcpkg
BUILD_DIR = build
BINARY_NAME = scuffcraft 

.PHONY: all configure build clean

all: build

configure:
	cmake -S . -B $(BUILD_DIR) \
		-DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake"

build: 
	cmake --build $(BUILD_DIR) --parallel

build-np: configure
	cmake --build $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: build
	./$(BUILD_DIR)/$(BINARY_NAME)