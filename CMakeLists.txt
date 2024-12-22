cmake_minimum_required(VERSION 3.26)
project(MultiClientChat C)

set(CMAKE_C_STANDARD 11)

add_executable(server server.c)
target_link_libraries(server ws2_32)

add_executable(client client.c)
target_link_libraries(client ws2_32)
