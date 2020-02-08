include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(sum sum_main.cpp)
add_executable(cksum cksum_main.cpp)
target_link_libraries(sum ${PROJECT_NAME})
target_link_libraries(cksum ${PROJECT_NAME})

