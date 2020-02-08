cmake_minimum_required(VERSION 3.5)

set(CMAKE_VERBOSE_MAKEFILE TRUE)

# Define the GTEST_DIR=<path> environment variable or
# set GTEST_DIR on command line: -DGTEST_DIR:STRING=<path>
if(NOT DEFINED GTEST_DIR)
    if(DEFINED ENV{GTEST_DIR})
        set(GTEST_DIR $ENV{GTEST_DIR})
    else()
        message(FATAL_ERROR "GTEST_DIR must be defined.")
    endif()
endif()

message(STATUS "Using GTEST_DIR=${GTEST_DIR}")
set(GTEST_INCLUDE ${GTEST_DIR}/include/)
set(GTEST_LIB ${GTEST_DIR}/build/lib)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/functional_streambuf/include
    ${GTEST_INCLUDE}
)

link_directories(
    ${GTEST_LIB}
)

add_executable(rehash_test rehash_test.cpp)

target_link_libraries(rehash_test
    ${PROJECT_NAME}
    gtest
    gtest_main
    pthread
)
