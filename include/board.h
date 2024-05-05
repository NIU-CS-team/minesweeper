#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "gl.h"

struct block {
    const int size = 8;
    int state = 0;  // 0: hidden, 1: revealed, 2: flagged
    int value = 0;  // 0: empty, 1-8: number of mines around, 9: mine
    double gl_x = 0;
    double gl_y = 0;
    int index = 0;
};

class Board {
public:
    enum game_status { PLAYING, WON, LOST };

    Board(int row, int col, int mines);
    ~Board();

    int show_all_mine();
    int timer();
    int get_input();
    int start_game();
    int print_board();
    int reveal(block target_block);
    int flagged(std::size_t x, std::size_t y);
    int remove_flagged(std::size_t x, std::size_t y);
    int flag_counter(int n_mines);

    int gl_init_board();
    int gl_draw_board(GLFWwindow *window);
    int gl_draw_block(GLFWwindow *window, block b);
    block gl_get_block(GLFWwindow *window, double x, double y);
    int gl_reveal(block target_block);
    int gl_flagged(int x, int y);
    int gl_remove_flagged(int x, int y);
    int gl_show_all_mine();

    const int border = 10;

    int row = 0;
    int col = 0;
    int n_mines;
    int n_flags = 0;
    int status = PLAYING;
    int n_revealed = 0;

    std::vector<block> blocks;
};

#endif