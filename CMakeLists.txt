cmake_minimum_required(VERSION 3.0)

project(HashMap)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 20)

#add_library(HashMapLib SHARED hashtable.cpp)

#target_link_libraries(HashMapLib)

add_executable(${PROJECT_NAME} main.cpp )
