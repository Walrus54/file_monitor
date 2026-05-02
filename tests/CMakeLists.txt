include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    test_signal.cpp    # Signal<T> — чистые юнит-тесты, нет ФС и потоков
    test_monitor.cpp   # FileMonitor — мок-чекеры, нет реальных файлов
    test_checkers.cpp  # FileChecker — изолированные тесты с /tmp
)
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(unit_tests PRIVATE core checkers notifiers utils GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(unit_tests)
