cmake_minimum_required(VERSION 3.25)
project(8042_project)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(GIS GIS.cpp)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
target_sources(GIS PRIVATE ${SOURCES})

# force copy data directory to build directory
file(GLOB DATA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/data/*")
file(COPY ${DATA_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})