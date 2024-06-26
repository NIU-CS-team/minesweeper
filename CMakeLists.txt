cmake_minimum_required(VERSION 3.20.0)
project(minesweeper VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 23)

include_directories(./include)

find_package(SFML COMPONENTS graphics window system network audio REQUIRED)

set(LIB_LINKS sfml-graphics sfml-window sfml-system sfml-network sfml-audio)

file(GLOB_RECURSE SRC_FILES ./src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_LINKS})

install(TARGETS ${PROJECT_NAME})