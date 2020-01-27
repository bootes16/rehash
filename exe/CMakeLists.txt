include_directories(${PROJECT_SOURCE_DIR}/include)

message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "_PROJECT_NAME: ${ghashlib_PROJECT_NAME}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")

add_executable(sum sum_main.cpp)
add_executable(cksum cksum_main.cpp)
target_link_libraries(sum ${PROJECT_NAME})
target_link_libraries(cksum ${PROJECT_NAME})

