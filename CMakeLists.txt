cmake_minimum_required(VERSION 3.5)

project(rehash CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(exe)
add_subdirectory(test)
