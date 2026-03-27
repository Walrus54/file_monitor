cmake_minimum_required(VERSION 3.16)
project(FileMonitor CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(file_monitor
    main.cpp
    src/FileMonitor.cpp
    src/ConsoleNotifier.cpp
    src/checkers/ExistenceChecker.cpp
    src/checkers/SizeChecker.cpp
)
