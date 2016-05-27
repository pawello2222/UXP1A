cmake_minimum_required(VERSION 3.3)
project(UXP1A)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Model/LindaTupleItemType.h Model/LindaTuple.cpp Model/LindaTuple.h Model/LindaTupleItem.cpp Model/LindaTupleItem.h Exception/UnknownLindaTupleType.cpp Exception/UnknownLindaTupleType.h Exception/InvalidOperation.cpp Exception/InvalidOperation.h IPC/LindaTuplePool.cpp IPC/LindaTuplePool.h Model/LindaTupleTemplate.cpp Model/LindaTupleTemplate.h Model/LindaTupleItemTemplate.cpp Model/LindaTupleItemTemplate.h)
add_executable(UXP1A ${SOURCE_FILES})