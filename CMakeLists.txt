cmake_minimum_required(VERSION 3.3)
project(UXP1A)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Matcher/Matcher.cpp Matcher/Matcher.h Model/LindaTupleItemType.h)
add_executable(UXP1A ${SOURCE_FILES})