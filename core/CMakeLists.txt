add_library(core STATIC
    src/FileMonitor.cpp
)
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(core PUBLIC utils)
