cmake_minimum_required(VERSION 4.0.0)

project(c4_ai)

set(CMAKE_BUILD_TYPE Release)

set(HEADERS game.hpp board.hpp slot.hpp pointer.hpp utils.hpp enums.hpp decision_tree.hpp)
set(SOURCE game.cpp main.cpp board.cpp slot.cpp pointer.cpp utils.cpp decision_tree.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCE})

add_subdirectory(external/raylib)

target_include_directories(${PROJECT_NAME} PUBLIC external/raylib/src)

target_link_libraries(${PROJECT_NAME} raylib)