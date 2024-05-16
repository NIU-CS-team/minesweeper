#include <filesystem>
#include <iostream>
#include <vector>

#include "board.h"
#include "gl.h"
#include "json.h"
#include "network.h"
#include <cassert>

int main(int argc, char* argv[]) {
    std::vector<int> config = read_json(std::filesystem::path("config.json"));

    Board game(8, 8, 10);

    GL gl;
    gl.init();
    // gl.main_menu();
    gl.init_board(game);
    gl.play_single(game);
    return 0;
}
