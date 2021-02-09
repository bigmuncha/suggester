cmake_minimum_required(VERSION 3.10)

project(suggester)

set(SOURCE_LIB self_cache.cpp)

add_library(self_cache STATIC ${SOURCE_LIB})
