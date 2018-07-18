project(SettingsContainer)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-O3 -Wall -std=c++14")
add_library(SETTINGS_CONTAINER_LIB STATIC Settings.h Settings.cpp)
