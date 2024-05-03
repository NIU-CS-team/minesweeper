#include <iostream>
#include <vector>
#include <filesystem>

#include "board.h"
#include "gl.h"
#include "network.h"
#include "json.h"

int main(int argc, char* argv[]) {
    std::vector<int> config = read_json(std::filesystem::path("config.json"));

    Board game(10, 10, 10);
    game.gl_init_board();
    return 0;
}
