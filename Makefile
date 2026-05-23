BUILD_DIR = build

all: build test

build:
	mkdir -p $(BUILD_DIR) \
	&& cd build \
	&& cmake .. \
	&& cmake --build . -j$$(nproc --ignore=1)

test:
	cd $(BUILD_DIR) \
	&& ctest -VV

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all build test clean
