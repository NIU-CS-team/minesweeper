#include <cassert>
#include <filesystem>
#include <iostream>
#include <vector>

#include "board.h"
#include "gl.h"
#include "json.h"
#include "network.h"

int main(int argc, char* argv[]) {
    std::vector<int> config = read_json(std::filesystem::path("config.json"));

    Board game(8, 8, 10);

    GL gl;
    gl.init();
    /*
    switch (gl.main_menu()) {
        case 1:
            host_game(config[0], config[1]);
            break;
        case 2:
            join_game(config[2], config[3]);
            break;
        case 3:
            gl.init_board(game);
            gl.play_single(game);
        default:
            assert(false);
    }
    */

    gl.init_board(game);
    gl.play_single(game);
    return 0;
}
