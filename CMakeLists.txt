cmake_minimum_required(VERSION 3.28)
project(CTorture4 C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(CTorture4 main.c src/functions/functions.c)
add_executable(tests tests.cpp src/functions/functions.c)
target_link_libraries(tests gtest_main)