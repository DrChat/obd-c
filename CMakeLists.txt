cmake_minimum_required(VERSION 3.14.1)

project(obd-c
    VERSION 0.0.1
)

include(GoogleTest)

if (MSVC)
    string(REGEX REPLACE " ?/W[0-4] ?" " " CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REGEX REPLACE " ?/W[0-4] ?" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    add_compile_options(
        /MP  # Multiprocessor compilation
        /W4  # Warning level 4
        /WX  # Treat warnings as errors
    )
endif()

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(third_party)
