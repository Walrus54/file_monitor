add_library(utils STATIC
    src/Logger.cpp
    src/Config.cpp
)
target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR})
