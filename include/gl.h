#ifndef GL_H
#define GL_H

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#endif

#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
// #include <glad/glad.h>
#endif

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#include <map>

#include "board.h"

class GL {
public:
    enum bomb_count_color {
        BLACK,
        BLUE,
        GREEN,
        RED,
        PURPLE,
        MAROON,
        TURQUOISE,
        GRAY,
        WHITE,
    };

    std::map<int, std::vector<float>> bomb_count_color_map = {
        {BLACK, {0.0f, 0.0f, 0.0f}},
        {BLUE, {0.0f, 0.0f, 1.0f}},
        {GREEN, {0.0f, 1.0f, 0.0f}},
        {RED, {1.0f, 0.0f, 0.0f}},
        {PURPLE, {1.0f, 0.0f, 1.0f}},
        {MAROON, {0.5f, 0.0f, 0.0f}},
        {TURQUOISE, {0.0f, 1.0f, 1.0f}},
        {GRAY, {0.5f, 0.5f, 0.5f}},
        {WHITE, {1.0f, 1.0f, 1.0f}},
    };

    int init();
    int init_board(std::vector<block> blocks, int row, int col, GLFWwindow* window);
    int draw_board(GLFWwindow* window, std::vector<block> blocks, int row, int col);
    int setup_block(block b, unsigned int& VBO, unsigned int& VAO, unsigned int& EBO);
    int draw_block(unsigned int VAO, block b);
    block get_block(GLFWwindow *window, double x, double y, int row, int col);
    int reveal(block target_block);
    int flagged(block target_block);
    int remove_flagged(block target_block);
    int show_all_mine(std::vector<block> blocks, int row, int col);
    int main_menu();

    GLFWwindow* window;
};

#endif