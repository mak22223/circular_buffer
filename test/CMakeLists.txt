option(ENABLE_COVERAGE "Enable coverage" OFF)

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

if(ENABLE_COVERAGE)
  target_compile_options(circular_buffer_test PRIVATE
    -O0
    -g
    --coverage
  )

  target_link_options(circular_buffer_test PRIVATE
    --coverage
  )
endif()

add_test(circular_buffer_tests circular_buffer_test)
