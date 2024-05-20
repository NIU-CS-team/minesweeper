#include "gl.h"

#include <GL/glew.h>

#include <iostream>

#include "block.h"
#include "board.h"
#include "io.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int GL::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(800, 800, "Minesweeper", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err)
                  << std::endl;
        return -1;
    }

    return 0;
}

int GL::setup_block(block b) {
    b.gl_x = ((b.index % 8) + 0.1f) * 2.0f / 8 - 1.0f;
    b.gl_y = ((static_cast<float>(b.index) / 8) + 0.1f) * 2.0f / 8 - 1.0f;

    glVertex2f(b.gl_x - b.size, b.gl_y - b.size);
    glVertex2f(b.gl_x + b.size, b.gl_y - b.size);
    glVertex2f(b.gl_x + b.size, b.gl_y + b.size);
    glVertex2f(b.gl_x - b.size, b.gl_y + b.size);

    return 0;
}

int GL::draw_board(Board board) {
    for (int i = 0; i < board.row * board.col; i++) {
        glBegin(GL_QUADS);
        this->setup_block(board.blocks[i]);
        glEnd();
        this->draw_block(board.blocks[i]);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
    return 0;
}

int GL::draw_block(block b) {
    if (bomb_count_color_map.find(b.value) == bomb_count_color_map.end()) {
        std::cerr << "錯誤: b.value: " << b.value
                  << " 不在 bomb_count_color_map 的範圍內" << std::endl;
        return -1;
    }

    std::vector<float> rgb = bomb_count_color_map[b.value];

    glColor3f(rgb[0], rgb[1], rgb[2]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    return 0;
}

block GL::get_block(Board board, double x, double y) {
    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    block target_block;
    y = (x / window_width) * 2.0 - 1.0;
    y = ((window_height - y) / window_height) * 2.0 - 1.0;

    target_block.index =
        static_cast<int>((y + 1.0) / 2.0 * board.col) * board.row +
        static_cast<int>((x + 1.0) / 2.0 * board.row);
    return target_block;
}

int GL::reveal(Board board, block target_block) {
    // if the block is already revealed or flagged, do nothing
    if (target_block.state != HIDDEN) {
        return 0;
    }

    target_block.state = REVEALED;

    // if the block is a mine, stop revealing
    if (target_block.value >= MINE) {
        return 0;
    }

    if (target_block.value == EMPTY) {
        if (!(target_block.index < board.row * board.col &&
              target_block.index >= 0)) {
            return 0;
        }

        for (int i = -1; i <= 1; i++) {
            if (target_block.index - board.row + i >= 0 &&
                target_block.index - board.row + i < board.row * board.col) {
                reveal(board, board.blocks[target_block.index - board.row + i]);
            }
        }

        if (target_block.index - 1 >= 0) {
            reveal(board, board.blocks[target_block.index - 1]);
        }

        if (target_block.index + 1 < board.row * board.col) {
            reveal(board, board.blocks[target_block.index + 1]);
        }

        for (int i = -1; i <= 1; i++) {
            if (target_block.index + board.row + i >= 0 &&
                target_block.index + board.row + i < board.row * board.col) {
                reveal(board, board.blocks[target_block.index + board.row + i]);
            }
        }
    }

    return 0;
}

int GL::show_all_mine(Board board) {
    for (auto i : board.blocks) {
        if (i.value >= 9) {
            i.state = REVEALED;
        }
    }

    this->draw_board(board);
    return 0;
}

int GL::main_menu() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, 0.2f);
    glVertex2f(-0.5f, 0.2f);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0.5f, 0.1f);
    glVertex2f(0.5f, -0.2f);
    glVertex2f(-0.5f, -0.2f);
    glVertex2f(-0.5f, 0.1f);
    glVertex2f(0.5f, -0.3f);
    glVertex2f(0.5f, -0.6f);
    glVertex2f(-0.5f, -0.6f);
    glVertex2f(-0.5f, -0.3f);
    glColor3f(0.0f, 1.0f, 2.0f);
    glEnd();

    glfwSwapBuffers(window);
    glfwPollEvents();
    // wait for user choose host or join
    while (!glfwWindowShouldClose(window)) {
        int button, action, mods;
        if (mouse_button_callback(window, button, action, mods) != 1) continue;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (xpos > 0.5 && xpos < -0.5 && ypos > 0.2 && ypos < 0.5) {
            return 1;
        } else if (xpos > 0.5 && xpos < -0.5 && ypos > -0.2 && ypos < 0.1) {
            return 2;
        } else if (xpos > 0.5 && xpos < -0.5 && ypos > -0.6 && ypos < -0.3) {
            return 3;
        }
    }

    // if user choose host, wait for another player to join
    // if user choose join, show ip address input box
}

int GL::play_single(Board board) {
    while (!glfwWindowShouldClose(window) && board.status == board.PLAYING) {
        draw_board(board);
        double xpos, ypos;  // 修改這裡
        int button, action, mods;
        mouse_button_callback(window, button, action, mods);
        // get cursorpos when mouse button is pressed
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            glfwGetCursorPos(window, &xpos, &ypos);  // 修改這裡
        }
        reveal(board, get_block(board, xpos, ypos));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
