VCPKG_ROOT ?= $(HOME)/vcpkg
BUILD_DIR = build
BINARY_NAME = scuffcraft
MODE ?= game

ifeq ($(MODE),face)
	APP_DEFINE = -DRUN_FACE_DETECTION
else ifeq ($(MODE),red)
	APP_DEFINE = -DRUN_RED_STATIC
else
	APP_DEFINE =
endif

.PHONY: all configure build clean run

all: build

configure:
	cmake -S . -B $(BUILD_DIR) \
		-DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake" \
		-DCMAKE_CXX_FLAGS="$(APP_DEFINE)"

build: configure
	cmake --build $(BUILD_DIR) --parallel

build-np: configure
	cmake --build $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: build
	./$(BUILD_DIR)/$(BINARY_NAME)
