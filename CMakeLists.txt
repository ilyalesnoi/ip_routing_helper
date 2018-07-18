cmake_minimum_required(VERSION 2.8)
project(ip_routing_helper)

set(PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH "${PROJECT_PATH}/cmake" ${CMAKE_MODULE_PATH})

set(Config++_FIND_REQUIRED TRUE)
include(FindLibConfig)

add_subdirectory(CommandsLib)
add_subdirectory(Settings)

set(CMAKE_CXX_FLAGS "-O3 -Wall -std=c++14")
if(CONFIG++_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lconfig++")
endif(CONFIG++_FOUND)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CommandsLib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Settings)
set(BIN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable(${PROJECT_NAME} ${BIN_SOURCE} ${CONFIG++_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} COMMAND_LIST_LIB SETTINGS_CONTAINER_LIB)
