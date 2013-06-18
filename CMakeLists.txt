cmake_minimum_required (VERSION 2.8)

project (DiaballikBot)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
set (CMAKE_CXX_FLAGS "-O2 -std=c++11 -Wall")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")

if (NOT TARGET DiaballikEngine)
	add_subdirectory(DiaballikEngine)
endif()

add_subdirectory (src)
