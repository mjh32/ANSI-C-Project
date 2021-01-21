cmake_minimum_required(VERSION 3.17)
project(cs23820_mjh32 C)

set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-stack-protector")
add_executable(cs23820_mjh32 main.c)
target_link_libraries(cs23820_mjh32 m)