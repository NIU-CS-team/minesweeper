#include <iostream>

#include "board.h"
#include "gl.h"
#include "network.h"

int main(int argc, char* argv[]) {
    Board game(10, 10, 10);
    while (game.status == Board::PLAYING) {
        system("clear");
        game.print_board();
        auto input = game.get_input();
        if (input.first == -1) {
            break;
        }

        int index = input.first + input.second * game.row;
        if (game.blocks[index].state == 0) {
            game.blocks[index].state = 1;
            game.n_revealed++;
            if (game.blocks[index].value == 9) {
                game.show_all_mine();
                break;
            }

            if (game.n_revealed == game.row * game.col - game.n_mines) {
                game.status = Board::WON;
                break;
            }
        }
    }

    return 0;
}
