cmake_minimum_required(VERSION 3.10)

project(SOI-lab-6)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(SOI-lab-6 ${SRC})