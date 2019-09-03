set(libobd_tests_SOURCES
    "fakeelm.cc"
)

add_executable(libobd_tests ${libobd_tests_SOURCES})
target_link_libraries(libobd_tests
    PRIVATE
        gtest gtest_main
        gmock

        libobd
)

gtest_discover_tests(libobd_tests)