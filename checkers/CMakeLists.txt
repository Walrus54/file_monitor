add_library(checkers STATIC
    src/ExistenceChecker.cpp
    src/SizeChecker.cpp
    src/RestorationChecker.cpp
)
target_include_directories(checkers PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(checkers PUBLIC core)
