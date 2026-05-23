BUILD_DIR = build
CMAKE_ARGS =

all: build build_test test

build_test: CMAKE_ARGS += -DBUILD_TESTING=ON
build_test: build

build:
	mkdir -p $(BUILD_DIR) \
	&& cd build \
	&& cmake .. $(CMAKE_ARGS) \
	&& cmake --build . -j$$(nproc --ignore=1)

test:
	cd $(BUILD_DIR) \
	&& ctest -VV

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all build test clean
