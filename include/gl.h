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
#include <vector>
#include <sys/types.h>

#include "board.h"
#include "font.h"

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

    bool left_button_pressed = false;
    bool right_button_pressed = false;

    std::map<GLchar, Character> Characters;
    FT_Library ft;
    FT_Face face;

    int init();
    int clear();
    int draw_board(Board& board);
    int draw_block(Board board, block target_block);
    block get_block(Board board, double x, double y);
    int reveal(Board& board, block& target_block);
    int fast_reveal(Board& board, block& target_block);
    int flagged(Board board, block& target_block);
    int remove_flagged(Board board,block& target_block);
    int show_all_mine(Board board);
    int main_menu();
    int play_single(Board board);
    int host_game(Board game, u_int16_t port, int max_member);
    int join_game(uint32_t host_address, uint16_t host_port);
    int end_game(Board board);

    GLFWwindow* window;
};

#endif