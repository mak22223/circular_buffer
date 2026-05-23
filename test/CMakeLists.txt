include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(circular_buffer_test
  test_basic_operations.cpp
)

target_link_libraries(circular_buffer_test PRIVATE
  circular_buffer
  GTest::gtest
  GTest::gtest_main
)

add_test(circular_buffer_tests circular_buffer_test)
