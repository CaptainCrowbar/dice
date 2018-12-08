cmake_minimum_required(VERSION 3.6)

project(Dice
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1 NOMINMAX=1 UNICODE=1 _UNICODE=1 WINVER=0x601 _WIN32_WINNT=0x601)
    add_compile_options(/EHsc /Gy /MP /sdl /utf-8 /W4 /WX)
else()
    add_compile_definitions(_REENTRANT=1 _XOPEN_SOURCE=700)
    add_compile_options(-march=ivybridge -mfpmath=sse -Wall -Wextra -Werror)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        add_compile_definitions(_DARWIN_C_SOURCE=1)
    endif()
endif()

add_executable(Dice
    dice.cpp
    dice-test.cpp
    rational.cpp
    rational-test.cpp
    unit-test.cpp
)
