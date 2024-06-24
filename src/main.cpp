#include <cassert>
#include <iostream>

#include "board.h"
#include "sfml.hpp"
#include "config.h"
#include "network.h"

int main(int argc, char* argv[]) {
    SFML game(WIDTH, HEIGHT, MINES);
    game.init();
    game.play_single();
    return 0;
}
