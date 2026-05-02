cmake_minimum_required(VERSION 3.16)
project(FileMonitor CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(LIB_SOURCES
    src/core/FileMonitor.cpp
    src/notifiers/ConsoleNotifier.cpp
    src/utils/Logger.cpp
    src/utils/Config.cpp
    src/checkers/ExistenceChecker.cpp
    src/checkers/SizeChecker.cpp
    src/checkers/RestorationChecker.cpp
)

add_library(file_monitor_lib STATIC ${LIB_SOURCES})

add_executable(file_monitor app/main.cpp)
target_link_libraries(file_monitor file_monitor_lib)

# Tests
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(unit_tests
    tests/core/test_monitor.cpp
    tests/checkers/test_checkers.cpp
    tests/checkers/test_restoration.cpp
)
target_link_libraries(unit_tests file_monitor_lib GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(unit_tests)
