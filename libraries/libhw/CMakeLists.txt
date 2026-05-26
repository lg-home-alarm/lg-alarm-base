cmake_minimum_required(VERSION 3.28)

project(hw VERSION 0.9.0 DESCRIPTION "Lib Hw")

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES *.cpp)

add_library(hw SHARED ${SOURCES})