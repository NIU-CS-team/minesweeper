#include <cassert>
#include <filesystem>
#include <iostream>
#include <vector>

#include "board.h"
#include "gl.h"
#include "config.h"
#include "network.h"

int main(int argc, char* argv[]) {
    Board game(HEIGHT, WIDTH, MINES);

    GL gl;
    gl.init();
    std::string ip;
    while (true) {
        int menuSelection = gl.main_menu();
        switch (menuSelection) {
            case 1:
                gl.clear();
                game.clear();
                game.generate_mines();
                gl.host_game(game, 6969, 2);
                break;
            case 2:
                std::cout << "Joining game, input ip: " << std::endl;
                std::cin >> ip;
                uint32_t host_address;
                inet_pton(AF_INET, ip.c_str(), &host_address);
                gl.join_game(host_address, 6969);
                break;
            case 3:
                gl.clear();
                game.clear();
                game.generate_mines();
                gl.play_single(game);
                break;
            default:
                assert(false);
        }
    }

    while (!glfwWindowShouldClose(gl.window)) {
        glfwPollEvents();
    }

    //gl.play_single(game);
    return 0;
}
