cmake_minimum_required(VERSION 3.20)
project(vyshkat CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(vyshkat app/main.cpp)
target_include_directories(vyshkat PRIVATE third_party)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(vyshkat_tests tests/smoke_test.cpp)
target_link_libraries(vyshkat_tests PRIVATE GTest::gtest_main GTest::gmock)
include(GoogleTest)
gtest_discover_tests(vyshkat_tests)