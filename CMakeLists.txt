cmake_minimum_required(VERSION 3.20.0)
project(minesweeper VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 23)

include_directories(./include)

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE SRC_FILES ./src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} SDL2::SDL2)
