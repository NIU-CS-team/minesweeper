#include <iostream>
#include <vector>
#include <filesystem>

#include "board.h"
#include "gl.h"
#include "network.h"
#include "json.h"

int main(int argc, char* argv[]) {
    std::vector<int> config = read_json(std::filesystem::path("config.json"));
    for (auto i : config) {
        std::cout << i << std::endl;
    }

    Board game(config[0], config[1], config[2]);
    game.start_game();

    return 0;
}
