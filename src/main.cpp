#include <cassert>
#include <iostream>

#include "board.h"
#include "sfml.hpp"
#include "config.h"
#include "network.h"

int main(int argc, char* argv[]) {
    SFML game(HEIGHT, WIDTH, MINES);
    game.init();
    return 0;
}
