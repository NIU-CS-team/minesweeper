#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <cstdint>
#include <SDL2/SDL.h>

struct block {
    const int8_t size = 50;
    int16_t x;
    int16_t y;
    SDL_Rect self = {x, y, size, size};
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
        int render_blocks(SDL_Renderer* renderer);

        uint8_t row;
        uint8_t col;
        uint16_t n_mines;
        int8_t n_flags;
        int8_t revealed;
        int8_t status;

        const int8_t border = 10;

    private:
        std::vector<uint16_t> board;
        std::vector<block> blocks;
};

#endif