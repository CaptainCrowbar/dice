cmake_minimum_required(VERSION 3.15)
# set(CMAKE_VERBOSE_MAKEFILE ON)

project(Dice
    VERSION 2.0
    LANGUAGES CXX
)

include(compilers.cmake)

set(app dice)
include_directories(.)
find_package(Threads REQUIRED)

add_library(${app}-objects OBJECT
    ${app}/rational.cpp
    ${app}/dice.cpp
)

add_executable(${app}
    ${app}/main.cpp
)

add_executable(${app}-test
    test/rational-test.cpp
    test/dice-test.cpp
    test/unit-test.cpp
)

target_link_libraries(${app}
    PRIVATE ${app}-objects
    PRIVATE Threads::Threads
)

target_link_libraries(${app}-test
    PRIVATE ${app}-objects
    PRIVATE Threads::Threads
)
