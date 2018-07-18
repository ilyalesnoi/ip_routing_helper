project(CommandListLib)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-O3 -Wall -std=c++14")

add_library(COMMAND_LIST_LIB STATIC Command.h CommandList.h CommandList.cpp)

