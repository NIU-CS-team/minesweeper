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
    while (true) {
        int menuSelection = gl.main_menu();
        switch (menuSelection) {
            case 1:
                host_game(config[0], config[1]);
                break;
            case 2:
                join_game(config[2], config[3]);
                break;
            case 3:
                glBegin(GL_QUADS);
                glVertex2f(1.0f, 1.0f);
                glVertex2f(1.0f, -1.0f);
                glVertex2f(-1.0f, -1.0f);
                glVertex2f(-1.0f, 1.0f);
                glColor3f(0.0f, 0.0f, 0.0f);
                glEnd();
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
