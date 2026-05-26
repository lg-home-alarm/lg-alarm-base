cmake_minimum_required(VERSION 3.28)
project(alarmhnd)

file(GLOB_RECURSE PROJECT_FILES "*.cpp")


file(GLOB_RECURSE HEADERS "*.h")
set(HEADER_DIRECTORIES)
foreach(header IN LISTS HEADERS)
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND HEADER_DIRECTORIES ${dir})
endforeach()

add_executable(alarmhnd ${PROJECT_FILES})

target_include_directories(alarmhnd PRIVATE ${HEADER_DIRECTORIES})