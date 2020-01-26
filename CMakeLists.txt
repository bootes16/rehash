cmake_minimum_required(VERSION 3.5)

project(rehash CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)

message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "_PROJECT_NAME: ${ghashlib_PROJECT_NAME}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
