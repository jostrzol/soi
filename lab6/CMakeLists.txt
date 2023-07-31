cmake_minimum_required(VERSION 3.10)

project(myfs)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(myfs ${SRC})