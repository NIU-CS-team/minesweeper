#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <cstdint>
#include <SDL2/SDL.h>

struct block {
    const int8_t size = 50;
    int16_t x;
    int16_t y;
    SDL_Rect self;

    block() : self{x, y, size, size} {}
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