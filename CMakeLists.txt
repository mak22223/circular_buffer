cmake_minimum_required(VERSION 3.15)
project(circular_buffer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(circular_buffer INTERFACE)

target_include_directories(circular_buffer INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include(CTest)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
