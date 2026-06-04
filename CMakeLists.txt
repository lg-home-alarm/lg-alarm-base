cmake_minimum_required(VERSION 3.28)
project(lg_alarm_central)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/output/debug/lg-alarm-central/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/output/debug/lg-alarm-central/lib)

set(IPCZMQ ON CACHE BOOL "" FORCE)

add_subdirectory(libraries/libhw)
add_subdirectory(libraries/coreLib)
add_subdirectory(executables/acqhnd)
add_subdirectory(executables/alarmhnd)
add_subdirectory(executables/networkhnd)
add_subdirectory(executables/notifierhnd)

add_executable(lg_alarm_central main.cpp)

list(APPEND HEADER_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/libraries/libhw
        ${CMAKE_SOURCE_DIR}/libraries/coreLib/executor
        ${CMAKE_SOURCE_DIR}/libraries/coreLib/ipc
        ${CMAKE_SOURCE_DIR}/libraries/ipc/ipczmq
        ${CMAKE_SOURCE_DIR}/vendor/cppzmq
        ${CMAKE_SOURCE_DIR}/vendor/libzmq/include)
target_include_directories(lg_alarm_central PRIVATE ${HEADER_DIRECTORIES})

target_link_libraries(lg_alarm_central PRIVATE hw ipczmq coreLib libzmq )



