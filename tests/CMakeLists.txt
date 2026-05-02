include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    test_monitor.cpp
    test_checkers.cpp
    test_restoration.cpp
)
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(unit_tests PRIVATE core checkers notifiers utils GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(unit_tests)
