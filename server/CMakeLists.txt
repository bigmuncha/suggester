cmake_minimum_required(VERSION 3.10)

project(suggester)

set(SOURCE1_LIB server.cpp)
set(SOURCE2_LIB primeserv.cpp)

find_package(Boost  REQUIRED COMPONENTS  filesystem thread regex)
if(Boost_FOUND)

  include_directories(${Boost_INCLUDE_DIRS} )

  add_library(server STATIC ${SOURCE1_LIB})
  target_link_libraries(server ${Boost_LIBRARIES})

  add_library(primeserv STATIC ${SOURCE2_LIB})
  target_link_libraries(primeserv ${Boost_LIBRARIES})
endif()
