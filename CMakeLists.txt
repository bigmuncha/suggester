cmake_minimum_required(VERSION 3.10)

project(suggester)

set(SOURCE_EXE main.cpp)
set(SERVER_LIB server.cpp)
#set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.75.0 REQUIRED COMPONENTS  filesystem thread)

if(Boost_FOUND)
  add_library(server ${SERVER_LIB})

  include_directories(${Boost_INCLUDE_DIRS} )

  add_executable(suggester ${SOURCE_EXE})

  target_link_libraries(suggester  server ${Boost_LIBRARIES} )
endif()
