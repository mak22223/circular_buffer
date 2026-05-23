BUILD_DIR = build
CMAKE_ARGS =
GCOVR_ARGS =

all: build build_test test coverage

build_test: CMAKE_ARGS += -DBUILD_TESTING=ON -DENABLE_COVERAGE=ON
build_test: build

build:
	mkdir -p $(BUILD_DIR) \
	&& cd build \
	&& cmake .. $(CMAKE_ARGS) \
	&& cmake --build . -j$$(nproc --ignore=1)

test: build_test
	cd $(BUILD_DIR) \
	&& ctest -VV

clean:
	rm -rf $(BUILD_DIR)

coverage: GCOVR_ARGS += --html-details coverage.html
coverage: coverage-ci

coverage-ci: test
	gcovr --root . --exclude test --exclude build \
	--xml coverage.xml $(GCOVR_ARGS)

.PHONY: all build build_test test clean coverage
