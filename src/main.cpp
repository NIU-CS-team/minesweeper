#include <filesystem>
#include <vector>

#include "board.h"
#include "gl.h"
#include "json.h"
#include "network.h"

int main(int argc, char* argv[]) {
    std::vector<int> config = read_json(std::filesystem::path("config.json"));

    Board game(8, 8, 10);
    GL gl;
    gl.main_menu();
    game.start_game();
    return 0;
}
