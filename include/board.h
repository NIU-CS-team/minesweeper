#ifndef BOARD_H
#define BOARD_H

#include <vector>

struct block {
    const int size = 50;
    int x;
    int y;
    int state = 0; // 0: hidden, 1: revealed, 2: flagged
    int value = 0; // 0: empty, 1-8: number of mines around, 9: mine
};

class Board {
    public:
        enum game_status {
            PLAYING,
            WON,
            LOST
        };

        Board(int row, int col, int mines);
        ~Board();

        int show_all_mine();
        int timer();
        std::pair<int, int> get_input();
        int start_game();
        int print_board();
        int flagged(size_t x, size_t y);
        int canceled(size_t x, size_t y);
        int flag_counter();

        const int border = 10;

        int row = 0;
        int col = 0;
        int n_mines;
        int n_flags = 0;
        int status = PLAYING;
        int revealed_blocks = 0;

        std::vector<block> blocks;

    private:
        std::vector<int> board;
};

#endif