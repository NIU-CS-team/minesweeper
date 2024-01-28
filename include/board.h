#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <cstdint>

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

        uint8_t row;
        uint8_t col;
        uint16_t n_mines;
        int8_t n_flags;
        int8_t revealed;
        int8_t status;

    private:
        std::vector<uint16_t> board;

};

#endif