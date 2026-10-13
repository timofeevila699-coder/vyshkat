cmake_minimum_required(VERSION 3.20)
project(vyshkat CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Модель: все .cpp, кроме main
add_library(vyshkat_core
    app/Base/InventoryBase/InventoryBase.cpp
    app/Base/EnergyBase/EnergyBase.cpp
    app/Base/EasyBase/EasyBase.cpp
    app/ServiceCenter/ServiceCenter.cpp
    app/Park/Park.cpp
    app/Report/Report.cpp
)
target_include_directories(vyshkat_core PUBLIC app third_party)
target_compile_options(vyshkat_core PRIVATE -Wall -Wextra)

# Консольное приложение
add_executable(vyshkat app/main.cpp app/Console/ConsoleApp.cpp)
target_link_libraries(vyshkat PRIVATE vyshkat_core)
target_compile_options(vyshkat PRIVATE -Wall -Wextra)
# Тесты
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(vyshkat_tests
    tests/service_center_test.cpp
    tests/validation_test.cpp
    tests/park_test.cpp
    tests/report_test.cpp
    tests/integration_test.cpp
)

option(ENABLE_COVERAGE "Сборка с покрытием" OFF)
if(ENABLE_COVERAGE)
    target_compile_options(vyshkat_core PRIVATE --coverage -O0)
    target_link_options(vyshkat_core PUBLIC --coverage)
    target_compile_options(vyshkat_tests PRIVATE --coverage -O0)
endif()

target_include_directories(vyshkat_tests PRIVATE tests)
target_link_libraries(vyshkat_tests PRIVATE vyshkat_core GTest::gtest_main GTest::gmock)
include(GoogleTest)
gtest_discover_tests(vyshkat_tests)