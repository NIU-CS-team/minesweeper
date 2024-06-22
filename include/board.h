#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <chrono>

#include "block.h"

class Board {
public:
    enum game_status { PLAYING, WON, LOST };

    Board(int row, int col, int mines);
    ~Board();

    int generate_mines();
    int clear();
    int show_all_mine();
    int get_input();
    int start_game();
    int print_board();
    int reveal(block& target_block);
    int flagged(block& target_block);
    int remove_flagged(block& target_block);
    int flag_counter(int n_mines);
    int check_win();

    int row = 0;
    int col = 0;
    int n_mines;
    int n_flags = 0;
    int status = PLAYING;
    int n_revealed = 0;

    bool first_move = true;

    std::vector<block> blocks;
    std::chrono::time_point<std::chrono::system_clock> start_time;
    std::chrono::time_point<std::chrono::system_clock> end_time;
};

#endif