cmake_minimum_required(VERSION 3.16)
project(FileMonitor CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(LIB_SOURCES
    src/FileMonitor.cpp
    src/ConsoleNotifier.cpp
    src/Logger.cpp
    src/Config.cpp
    src/checkers/ExistenceChecker.cpp
    src/checkers/SizeChecker.cpp
)

add_library(file_monitor_lib STATIC ${LIB_SOURCES})

add_executable(file_monitor main.cpp)
target_link_libraries(file_monitor file_monitor_lib)
