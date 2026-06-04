cmake_minimum_required(VERSION 3.28)

project(ipczmq VERSION 0.9.0 DESCRIPTION "Executor Lib")

set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(WITH_PERF_TOOL OFF CACHE BOOL "" FORCE)
set(WITH_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED ON CACHE BOOL "" FORCE)
set(BUILD_STATIC OFF CACHE BOOL "" FORCE)
set(ZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/libzmq
        ${CMAKE_BINARY_DIR}/libzmq-build)

set_target_properties(libzmq PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/output/debug/lg-alarm-central/lib/vendor)

add_library(ipczmq SHARED IPCZmq.cpp)

target_link_libraries(ipczmq PRIVATE libzmq coreLib)
target_include_directories(ipczmq PRIVATE
        ${CMAKE_SOURCE_DIR}/libraries/coreLib/ipc
        ${CMAKE_SOURCE_DIR}/vendor/cppzmq
        ${CMAKE_SOURCE_DIR}/vendor/libzmq/include)