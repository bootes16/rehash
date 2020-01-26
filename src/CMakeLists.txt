include_directories(${PROJECT_SOURCE_DIR}/include)
set(REHASH_SOURCE
    bsd_sum.cpp
    sysv_sum.cpp
    cksum.cpp
)

message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "_PROJECT_NAME: ${ghashlib_PROJECT_NAME}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")

add_library(${PROJECT_NAME} STATIC ${REHASH_SOURCE})
