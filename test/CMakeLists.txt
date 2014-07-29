# Must set version 2.8.1 or later to use relative path for library link
cmake_minimum_required(VERSION 2.8.1)

project(ofchecker C)

# build options for release
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wextra -O2")

# build options for debug
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wextra -g")

link_directories(../src)

add_executable(ofc_test test.c)
target_link_libraries(ofc_test ofc)

