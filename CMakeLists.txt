cmake_minimum_required(VERSION 3.10)

project(suggester)

set(SOURCE_EXE main.cpp)

include_directories(server cache)

add_executable(suggester ${SOURCE_EXE})

add_subdirectory(server)
add_subdirectory(cache)

target_link_libraries(suggester primeserv server self_cache)
