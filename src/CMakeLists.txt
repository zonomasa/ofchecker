# Must set version 2.8.1 or later to use relative path for library link
cmake_minimum_required(VERSION 2.8.1)

project(ofchecker C)

# build options for default
set(CMAKE_C_FLAGS "-Wall -Wextra -O2 -g")

# build options for release
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wextra -O2 -g")

# build options for debug
set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -g")

add_library(ofcbt SHARED backtrace.c)

add_library(ofc SHARED ofchecker.c)

target_link_libraries(ofc ${CMAKE_DL_LIBS})
target_link_libraries(ofc ofcbt)


