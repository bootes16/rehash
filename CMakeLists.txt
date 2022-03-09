cmake_minimum_required(VERSION 3.5)

project(rehash CXX)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(ENV{GTEST_DIR} "./googletest/googletest/")

include_directories(${PROJECT_SOURCE_DIR}/include)
set(GTEST_DIR=${PROJECT_SOURCE_DIR}/googletest/googletest/include)

add_subdirectory(src)
add_subdirectory(exe)
add_subdirectory(simple_prng)
add_subdirectory(test)
add_subdirectory(googletest)
