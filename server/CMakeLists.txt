cmake_minimum_required(VERSION 3.10)

project(server)

set(SOURCE_LIB server.cpp)

find_package(Boost 1.75.0 REQUIRED COMPONENTS  filesystem thread)
if(Boost_FOUND)

  include_directories(${Boost_INCLUDE_DIRS} )

  add_library(server STATIC ${SOURCE_LIB})
  target_link_libraries(server ${Boost_LIBRARIES})
endif()
