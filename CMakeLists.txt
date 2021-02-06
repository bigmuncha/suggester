cmake_minimum_required(VERSION 3.10)

project(suggester)

set(SOURCE_EXE main.cpp)

include_directories(server)

add_executable(suggester ${SOURCE_EXE})

add_subdirectory(server)

target_link_libraries(suggester server)
