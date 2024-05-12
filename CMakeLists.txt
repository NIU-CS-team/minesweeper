cmake_minimum_required(VERSION 3.20.0)
project(minesweeper VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 23)

include_directories(./include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)

include_directories(./include ${FREETYPE_INCLUDE_DIRS})

include_directories(/usr/local/include/freetype2)
include_directories(/usr/include/freetype2)

set(LIB_LINKS
    OpenGL::GL
    glfw
    GLEW::GLEW
    -lfreetype
)

file(GLOB_RECURSE SRC_FILES ./src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_LINKS})
