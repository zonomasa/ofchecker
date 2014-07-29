# Must set version 2.8.1 or later to use relative path for library link
cmake_minimum_required(VERSION 2.8.1)
project(ofchecker C)
set(serial "0.0.1")

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
add_test(main test/ofc_test)

