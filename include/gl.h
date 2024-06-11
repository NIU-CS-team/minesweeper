#ifndef GL_H
#define GL_H

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
// #include <glad/glad.h>
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#include <map>

#include "board.h"

class GL {
public:
    enum bomb_count_color {
        GRAY,
        BLUE,
        GREEN,
        RED,
        INDIGO,
        MAROON,
        TURQUOISE,
        PURPLE,
        BLACK,
        WHITE,
    };

    std::map<int, std::vector<float>> bomb_count_color_map = {
        {BLACK, {0.0f, 0.0f, 0.0f}},     {BLUE, {0.0f, 0.0f, 1.0f}},
        {GREEN, {0.0f, 1.0f, 0.0f}},     {RED, {1.0f, 0.0f, 0.0f}},
        {INDIGO, {0.29f, 0.0f, 0.51f}},  {MAROON, {0.5f, 0.0f, 0.0f}},
        {TURQUOISE, {0.0f, 1.0f, 1.0f}}, {GRAY, {0.5f, 0.5f, 0.5f}},
        {PURPLE, {1.0f, 0.0f, 1.0f}},    {WHITE, {1.0f, 1.0f, 1.0f}},
    };

    int init();
    int draw_board(Board board);
    int draw_board(Board board);
    block get_block(Board board, double x, double y);
    int reveal(Board& board, block& target_block);
    int flagged(Board board, block& target_block);
    int remove_flagged(Board board,block& target_block);
    int show_all_mine(Board board);
    int main_menu();
    int play_single(Board board);

    GLFWwindow* window;
};

#endif