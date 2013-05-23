cmake_minimum_required (VERSION 2.8)

project (DiaballikBot)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set (CMAKE_CXX_FLAGS "-O2 -std=c++11")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")

add_subdirectory(DiaballikEngine)
add_subdirectory (src)
