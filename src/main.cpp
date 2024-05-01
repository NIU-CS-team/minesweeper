#include <iostream>

#include "board.h"
#include "gl.h"
#include "network.h"

int main(int argc, char* argv[]) {
    Board game(10, 10, 10);
    game.start_game();

    return 0;
}
