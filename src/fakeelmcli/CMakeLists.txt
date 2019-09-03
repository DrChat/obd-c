set(fakeelmcli_SOURCES
    "main.cc"
)

add_executable(fakeelmcli ${fakeelmcli_SOURCES})
target_link_libraries(fakeelmcli
    PRIVATE
        libobd
)
