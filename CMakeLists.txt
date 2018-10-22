cmake_minimum_required(VERSION 3.12)
project(matrix CXX)

set(CMAKE_CXX_STANDARD 11)

set(HEADERS_DIR "${CMAKE_SOURCE_DIR}/include")

add_subdirectory(test)
add_subdirectory(bench)
