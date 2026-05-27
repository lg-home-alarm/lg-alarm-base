cmake_minimum_required(VERSION 3.28)
project(lg_alarm_central)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/output/debug/lg-alarm-central/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/output/debug/lg-alarm-central/lib)

add_subdirectory(libraries/libhw)
add_subdirectory(libraries/coreLib)
add_subdirectory(executables/acqhnd)
add_subdirectory(executables/alarmhnd)
add_subdirectory(executables/networkhnd)
add_subdirectory(executables/notifierhnd)


set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(WITH_PERF_TOOL OFF CACHE BOOL "" FORCE)
set(WITH_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED ON CACHE BOOL "" FORCE)
set(BUILD_STATIC OFF CACHE BOOL "" FORCE)
set(ZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/libzmq)

set_target_properties(libzmq PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/output/debug/lg-alarm-central/lib/vendor)

#add_executable(lg_alarm_central main.cpp)

#list(APPEND HEADER_DIRECTORIES
#        ${CMAKE_SOURCE_DIR}/libraries/libhw
#        ${CMAKE_SOURCE_DIR}/libraries/coreLib/executor)
#target_include_directories(lg_alarm_central PRIVATE ${HEADER_DIRECTORIES})

#target_link_libraries(lg_alarm_central PRIVATE hw coreLib)



