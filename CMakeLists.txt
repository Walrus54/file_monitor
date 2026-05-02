cmake_minimum_required(VERSION 3.16)
project(FileMonitor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory(utils)
add_subdirectory(core)
add_subdirectory(checkers)
add_subdirectory(notifiers)

add_executable(file_monitor main.cpp)
target_include_directories(file_monitor PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(file_monitor PRIVATE core checkers notifiers utils)

option(FILE_MONITOR_BUILD_TESTS "Build unit tests" OFF)
if(FILE_MONITOR_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
