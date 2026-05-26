cmake_minimum_required(VERSION 3.28)

project(coreLib VERSION 0.9.0 DESCRIPTION "Executor Lib")

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES *.cpp)

file(GLOB_RECURSE HEADERS *.h)
set(HEADER_DIRECTORIES)
foreach(header IN LISTS HEADERS)
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND HEADER_DIRECTORIES ${dir})
endforeach()

add_library(coreLib SHARED ${SOURCES})

target_include_directories(coreLib PRIVATE ${HEADER_DIRECTORIES})