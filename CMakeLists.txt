cmake_minimum_required(VERSION 3.3)
project(UXP1A)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Model/LindaTupleItemType.h Model/LindaTuplesFileEntry.h Model/LindaTuple.cpp Model/LindaTuple.h Model/LindaTupleItem.cpp Model/LindaTupleItem.h Exception/UnknownLindaTupleType.cpp Exception/UnknownLindaTupleType.h Exception/InvalidOperation.cpp Exception/InvalidOperation.h IPC/LindaTuplePool.cpp IPC/LindaTuplePool.h Model/LindaTupleTemplate.cpp Model/LindaTupleTemplate.h Model/LindaTupleItemTemplate.cpp Model/LindaTupleItemTemplate.h Exception/LindaTuplePoolConnectionError.cpp Exception/LindaTuplePoolConnectionError.h Exception/LockingError.cpp Exception/LockingError.h Exception/ErrorCodeMessageExceptionBase.cpp Exception/ErrorCodeMessageExceptionBase.h Exception/LindaTupleMaxSizeExceeded.cpp Exception/LindaTupleMaxSizeExceeded.h Exception/LindaFileCorrupt.cpp Exception/LindaFileCorrupt.h Exception/FileOperationError.cpp Exception/FileOperationError.h Model/LindaTupleItemOperator.h Exception/UnknownLindaTupleOperator.h Exception/UnknownLindaTupleOperator.cpp
        ExpressionParser/Lexer.h ExpressionParser/Lexer.cpp ExpressionParser/Parser.h ExpressionParser/Parser.cpp
        ExpressionParser/Tag.h ExpressionParser/Token.h
        ExpressionParser/Word.h ExpressionParser/Real.h ExpressionParser/Number.h ExpressionParser/Identifier.h)
add_executable(UXP1A ${SOURCE_FILES})