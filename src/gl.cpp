#include "gl.h"

#include <GL/glew.h>

#include <iostream>
#include <utility>

#include "block.h"
#include "board.h"

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

int GL::draw_board(Board& board) {
    float blockSizeInGL = 2.0 / std::max(board.col, board.row) - 0.01f;
    float width = board.col > board.row ? board.col : board.row;
    for (auto& block : board.blocks) {
        int index = block.index - 1;
        if (index == -1) index = board.col * board.row - 1;
        block.gl_x = ((index % board.col) / width) * 2.0 - 1.0;
        block.gl_y = ((index / board.col) / width) * 2.0 - 1.0;
    }

    glBegin(GL_QUADS);
    for (auto block : board.blocks) {
        glVertex2f(block.gl_x, block.gl_y);
        glVertex2f(block.gl_x + blockSizeInGL, block.gl_y);
        glVertex2f(block.gl_x + blockSizeInGL, block.gl_y + blockSizeInGL);
        glVertex2f(block.gl_x, block.gl_y + blockSizeInGL);

        if (block.state == REVEALED) {
            std::vector<float> rgb = bomb_count_color_map[block.value];
            glColor3f(rgb[0], rgb[1], rgb[2]);
        } else if (block.state == FLAGGED) {
            glColor3f(1.0f, 1.0f, 0.0f);
        } else {
            glColor3f(0.3f, 0.3f, 0.3f);
        }
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glEnd();

    glfwSwapBuffers(window);
    glfwPollEvents();
    return 0;
}

int GL::draw_block(Board board, block target_block) {
    float blockSizeInGL = 2.0 / std::max(board.col, board.row) - 0.01f;

    glBegin(GL_QUADS);
    glVertex2f(target_block.gl_x, target_block.gl_y);
    glVertex2f(target_block.gl_x + blockSizeInGL, target_block.gl_y);
    glVertex2f(target_block.gl_x + blockSizeInGL,
               target_block.gl_y + blockSizeInGL);
    glVertex2f(target_block.gl_x, target_block.gl_y + blockSizeInGL);

    if (target_block.state == REVEALED) {
        std::vector<float> rgb = bomb_count_color_map[target_block.value];
        glColor3f(rgb[0], rgb[1], rgb[2]);
    } else if (target_block.state == FLAGGED) {
        glColor3f(1.0f, 1.0f, 0.0f);
    } else {
        glColor3f(0.3f, 0.3f, 0.3f);
    }

    glEnd();

    glfwSwapBuffers(window);
    glfwPollEvents();
    return 0;
}

block GL::get_block(Board board, double x, double y) {
    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    block temp;
    float width = board.col > board.row ? board.col : board.row;
    temp.gl_x = (x / window_width) * 2.0 - 1.0;
    temp.gl_y = ((window_height - y) / window_height) * 2.0 - 1.0;

    temp.index = static_cast<int>((temp.gl_y + 1.0) / 2.0 * width) * board.col +
                 static_cast<int>((temp.gl_x + 1.0) / 2.0 * width);

    if (temp.index < 0 || temp.index >= board.row * board.col) {
        temp.index = -1;
    }

    return temp;
}

int GL::reveal(Board& board, block& target_block) {
    // if the block is already revealed or flagged, do nothing
    if (target_block.state != HIDDEN) {
        return 0;
    }

    target_block.state = REVEALED;
    board.n_revealed++;
    draw_block(board, target_block);

    // if the block is a mine, stop revealing
    if (target_block.value >= MINE) {
        board.status = Board::LOST;
        show_all_mine(board);

        return 0;
    }

    if (target_block.value == EMPTY) {
        for (int i = -1; i <= 1; i++) {
            if (target_block.index < board.row && i == -1) continue;
            if (target_block.index >= board.row * (board.col - 1) && i == 1)
                continue;

            for (int j = -1; j <= 1; j++) {
                if (target_block.index % board.row == 0 && j == -1) continue;
                if (target_block.index % board.row == board.row - 1 && j == 1)
                    continue;
                if (i == 0 && j == 0) continue;
                reveal(board,
                       board.blocks[target_block.index + i * board.row + j]);
            }
        }
    }

    return 0;
}

int GL::show_all_mine(Board board) {
    using namespace std::chrono;
    board.end_time = system_clock::now();

    for (auto i : board.blocks) {
        if (i.value >= 9) {
            i.state = REVEALED;
        }
    }

    duration<double> elapsed =
        duration_cast<microseconds>(board.end_time - board.start_time);
    // std::cout << "End time: (" << elapsed.count() << "s)\n";

    this->draw_board(board);
    return 0;
}

int GL::main_menu() {
    glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, 0.2f);
    glVertex2f(-0.5f, 0.2f);
    glVertex2f(-0.5f, 0.5f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.5f, 0.1f);
    glVertex2f(0.5f, -0.2f);
    glVertex2f(-0.5f, -0.2f);
    glVertex2f(-0.5f, 0.1f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.5f, -0.3f);
    glVertex2f(0.5f, -0.6f);
    glVertex2f(-0.5f, -0.6f);
    glVertex2f(-0.5f, -0.3f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glEnd();

    glfwSwapBuffers(window);
    glfwPollEvents();
    // wait for user choose host or join
    while (!glfwWindowShouldClose(window)) {
        double xpos, ypos;
        glfwPollEvents();
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            left_button_pressed = true;
        }
        if (left_button_pressed &&
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
            glfwGetCursorPos(window, &xpos, &ypos);
            left_button_pressed = false;
            if (xpos >= 150 && xpos <= 450 && ypos >= 480 && ypos <= 600) {
                return 1;
            } else if (xpos >= 150 && xpos <=450 && ypos >= 320 && ypos <= 440) {
                return 2;
            } else if (xpos >= 150 && xpos <= 450 && ypos >= 160 && ypos <= 280) {
                return 3;
            }
        }
    }
    // if user choose host, wait for another player to join
    // if user choose join, show ip address input box
    // std::unreachable();
    return 0;
}

int GL::play_single(Board board) {
    while (!glfwWindowShouldClose(window) && board.status == board.PLAYING) {
        draw_board(board);
        double xpos, ypos;
        glfwPollEvents();

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            left_button_pressed = true;
        }

        if (left_button_pressed &&
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ==
                GLFW_RELEASE) {
            glfwGetCursorPos(window, &xpos, &ypos);
            left_button_pressed = false;  // 重置按鍵狀態
            int target_block_index = get_block(board, xpos, ypos).index;
            if (target_block_index != -1) {
                reveal(board, board.blocks[target_block_index]);
                glfwSwapBuffers(window);
            }
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            right_button_pressed = true;
        }

        if (right_button_pressed &&
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) ==
                GLFW_RELEASE) {
            glfwGetCursorPos(window, &xpos, &ypos);
            right_button_pressed = false;  // 重置按鍵狀態
            int target_block_index = get_block(board, xpos, ypos).index;
            if (target_block_index != -1) {
                if (board.blocks[target_block_index].state == HIDDEN) {
                    board.blocks[target_block_index].state = FLAGGED;
                } else if (board.blocks[target_block_index].state == FLAGGED) {
                    board.blocks[target_block_index].state = HIDDEN;
                }
                draw_block(board, board.blocks[target_block_index]);
                glfwSwapBuffers(window);
            }
        }
    }

    if (board.status != board.PLAYING) {
        main_menu();
    }

    return 0;
}

int GL::flagged(Board board, block& target_block) {
    target_block.index = FLAGGED;
    this->draw_block(board, target_block);
    return 0;
}

int GL::remove_flagged(Board board, block& target_block) {
    target_block.index = HIDDEN;
    this->draw_block(board, target_block);
    return 0;
}
