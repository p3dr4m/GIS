cmake_minimum_required(VERSION 3.23)
project (finalprojalgo)

set(CMAKE_CXX_STANDARD 14)

add_executable(finalprojalgo main.cpp GIS.cpp GISRecord.cpp GISRecord.h NameIndex.cpp NameIndex.h CoordinateIndex.cpp CoordinateIndex.h BufferPool.cpp BufferPool.h Logger.cpp Logger.h SystemManager.cpp SystemManager.h CommandProcessor.cpp CommandProcessor.h)
