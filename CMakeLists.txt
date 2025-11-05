cmake_minimum_required(VERSION 3.10)
project(ctok LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CTOK_BUILD_TESTS "Enable tests" ON)

include(cmake/ZED.cmake)
include(cmake/pcre2.cmake)

add_subdirectory(src)

if(CTOK_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
