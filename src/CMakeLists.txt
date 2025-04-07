cmake_minimum_required(VERSION 3.22.1)

project(c4_ai)

add_subdirectory(external/raylib)

add_executable(${PROJECT_NAME} main.cpp)
