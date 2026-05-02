add_library(checkers STATIC
    src/FileChecker.cpp
)
target_include_directories(checkers PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(checkers PUBLIC core)
