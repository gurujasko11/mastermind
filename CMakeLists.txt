cmake_minimum_required(VERSION 2.8)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/usr/include/boost")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/include/boost/lib")

project(mastermind)
add_subdirectory(src)
include_directories(src)
add_subdirectory(test)