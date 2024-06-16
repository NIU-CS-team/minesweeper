#include "gl.h"

#include <GL/glew.h>

#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>

#include "block.h"
#include "board.h"
#include "network.h"
#include "config.h"
#include "font.h"

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

    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Failed to initialize FreeType" << std::endl;
        return -1;
    }

    if (FT_New_Face(ft, "font/JetBrainsMono.ttf", 0, &face)) {
        std::cerr << "Failed to load font" << std::endl;
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, 24);
    return 0;
}

int GL::clear() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    return 0;
}

int GL::draw_board(Board& board) {
    clear();
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

    if (target_block.state == REVEALED && target_block.value > 0) {
        render_text(face, std::to_string(target_block.value), target_block.gl_x,
                    target_block.gl_y, 0.02f, 1.0f, 1.0f, 1.0f, Characters);
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
    if (target_block.state == FLAGGED) {
        return 0;
    }

    if (target_block.state == REVEALED) {
        fast_reveal(board, target_block);
        return 0;
    }

    if (board.first_move) {
        board.first_move = false;
        board.start_time = std::chrono::system_clock::now();
        // board.generate_mines(target_block.index);
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

int GL::fast_reveal(Board& board, block& target_block) {
    if (target_block.state != REVEALED) {
        return 0;
    }

    int n_flagged = 0;
    for (int i = -1; i <= 1; i++) {
        if (target_block.index < board.row && i == -1) continue;
        if (target_block.index >= board.row * (board.col - 1) && i == 1)
            continue;

        for (int j = -1; j <= 1; j++) {
            if (target_block.index % board.row == 0 && j == -1) continue;
            if (target_block.index % board.row == board.row - 1 && j == 1)
                continue;
            if (i == 0 && j == 0) continue;

            if (board.blocks[target_block.index + i * board.row + j].state ==
                FLAGGED) {
                n_flagged++;
            }
        }
    }

    if (n_flagged == target_block.value) {
        for (int i = -1; i <= 1; i++) {
            if (target_block.index < board.row && i == -1) continue;
            if (target_block.index >= board.row * (board.col - 1) && i == 1)
                continue;

            for (int j = -1; j <= 1; j++) {
                if (target_block.index % board.row == 0 && j == -1) continue;
                if (target_block.index % board.row == board.row - 1 && j == 1)
                    continue;
                if (i == 0 && j == 0) continue;

                if (board.blocks[target_block.index + i * board.row + j].state ==
                    HIDDEN) {
                    reveal(board,
                           board.blocks[target_block.index + i * board.row + j]);
                }
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
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        if (left_button_pressed &&
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ==
                GLFW_RELEASE) {
            glfwGetCursorPos(window, &xpos, &ypos);
            left_button_pressed = false;
            int target_block_index = get_block(board, xpos, ypos).index;
            if (target_block_index != -1) {
                reveal(board, board.blocks[target_block_index]);
                glfwSwapBuffers(window);
            }

            board.check_win();
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            right_button_pressed = true;
        }

        if (right_button_pressed &&
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) ==
                GLFW_RELEASE) {
            glfwGetCursorPos(window, &xpos, &ypos);
            right_button_pressed = false;
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

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    if (board.status != board.PLAYING) {
        main_menu();
    }

    return 0;
}

int GL::host_game(Board game, u_int16_t port, int max_member) {
    max_member -= 1;  // exclude host

    // AF_INET: use IPv4
    // SOCK_DGRAM: UDP, SOCK_STREAM: TCP
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_address;

    if (socket_fd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return SOCKET_CREATE_FAILED;
    }

    // server connection information
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;  // vaild address accept
    server_address.sin_port = htons(port);        // use specify (include) port

    // bind port on host
    if (bind(socket_fd, (const struct sockaddr *)&server_address,
             sizeof(server_address)) < 0) {
        close(socket_fd);
        std::cerr << "Failed to bind port" << std::endl;
        return BIND_FAILED;
    } else {
        std::cout << "Bind port successfully" << std::endl;
    }

    // listen init
    if (listen(socket_fd, max_member) == SOCKET_CREATE_FAILED) {
        close(socket_fd);
        std::cerr << "Failed to listen" << std::endl;
        return LISTEN_ERROR;
    } else {
        std::cout << "Listening..." << std::endl;
    }

    if (close(socket_fd) < 0) {
        std::cerr << "Failed to close socket" << std::endl;
        return SOCKET_CLOSE_ERROR;
    } else {
        std::cout << "Socket closed" << std::endl;
    }

    // client connection information
    int reply_sockfd;
    sockaddr_in client_address;
    int client_index = 0;
    unsigned int client_len = sizeof(client_address);

    bool game_started = false;

    while (!game_started) {
        reply_sockfd =
            accept(socket_fd, (struct sockaddr *)&client_address, &client_len);
    }

    // send board information to client
    send(socket_fd, &game, sizeof(game), 0);
    // need consider client accept later disconnect

    // 遊戲互動環節
    game_data data;
    game_interaction(socket_fd, &data, &game);

    if (close(socket_fd) < 0) {
        return SOCKET_CLOSE_ERROR;
    }

    return SUCESS;
}

int GL::join_game(uint32_t host_address, uint16_t host_port) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        return SOCKET_CREATE_FAILED;
    }

    // check if the host address is valid
    if (host_address == 0) {
        close(socket_fd);
        std::cerr << "Invalid host address" << std::endl;
        return CONNECT_FAILED;
    }

    // server information
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = host_address;
    server_address.sin_port = htons(host_port);
    int server_len = sizeof(server_address);

    // try connect to server
    if (connect(socket_fd, (struct sockaddr *)&server_address, server_len) ==
        SOCKET_CREATE_FAILED) {
        close(socket_fd);
        return CONNECT_FAILED;
    }

    Board board(HEIGHT, WIDTH, MINES);
    char board_buffer[sizeof(Board)];
    ssize_t bytes_received =
        recv(socket_fd, board_buffer, sizeof(board_buffer), 0);
    if (bytes_received != sizeof(board_buffer)) {
        close(socket_fd);
        return MESSENGE_RECV_ERROR;
    }

    std::memcpy(&board, board_buffer, sizeof(Board));

    game_data data;
    game_interaction(socket_fd, &data, &board);

    if (close(socket_fd) < 0) {
        return SOCKET_CLOSE_ERROR;
    }

    return SUCESS;
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
