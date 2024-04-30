#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <cstdint>

struct block {
    const int8_t size = 50;
    int16_t x;
    int16_t y;
    int8_t state = 0; // 0: hidden, 1: revealed, 2: flagged
    int8_t value = 0; // 0: empty, 1-8: number of mines around, 9: mine
};

class Board {
    public:
        enum game_status {
            PLAYING,
            WON,
            LOST
        };

        Board(uint8_t row, uint8_t col, uint16_t mines);
        ~Board();

        int show_all_mine();
        int timer();

        const int8_t border = 10;

        uint8_t row = 8;
        uint8_t col = 8;
        uint16_t n_mines = 10;
        int8_t n_flags = 0;
        int8_t revealed = 0;
        int8_t status;

        std::vector<block> blocks;

    private:
        std::vector<uint16_t> board;
};

#endif