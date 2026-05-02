add_library(notifiers STATIC
    src/ConsoleNotifier.cpp
)
target_include_directories(notifiers PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(notifiers PUBLIC core utils)
