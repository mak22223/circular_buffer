cmake_minimum_required(VERSION 3.15)
project(circular_buffer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find_package(GTest REQUIRED)

enable_testing()

add_executable(circularbuffer_test
  test/integration_test.cpp
  test/test_basic_operations.cpp
)

target_include_directories(circularbuffer_test PUBLIC
  src
)

target_link_libraries(circularbuffer_test PUBLIC
  gtest
  gtest_main
)

add_test(circularbuffer_tests circularbuffer_test)
