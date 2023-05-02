cmake_minimum_required(VERSION 3.25)
project(8042_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(GIS)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
target_sources(GIS PRIVATE ${SOURCES})

# force copy data directory to build directory
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
