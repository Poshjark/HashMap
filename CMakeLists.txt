cmake_minimum_required(VERSION 3.0)

project(HashMap)

add_executable(${PROJECT_NAME} main.cpp )

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
