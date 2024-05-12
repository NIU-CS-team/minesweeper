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
    // game.gl_main_menu();
    
    GL gl;
    int result = gl.main_menu();
    assert(result == 0);
    return 0;
}
