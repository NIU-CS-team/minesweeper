#include "board.h"

#include <chrono>
#include <iostream>
#include <random>

#include "gl.h"

Board::Board(int row, int col, int n_mines)
    : row(row), col(col), n_mines(n_mines) {
    blocks.resize(row * col);
    this->col = col;
    this->row = row;
    // Generate Mines Locations

    std::random_device rd;   // 取得隨機數種子
    std::mt19937 gen(rd());  // 使用 Mersenne Twister 引擎
    std::uniform_int_distribution<int> dis(0, this->row * this->col - 1);
    for (int i = 0; i < this->n_mines; i++) {
        int rand = dis(gen);
        if (this->blocks[rand].value >= 9) {
            i--;
            continue;
        }

        this->blocks[rand].value = 9;
        for (int j = -1; j <= 1; j++) {
            if (rand < row && j == -1) continue;
            if (rand >= row * (col - 1) && j == 1) continue;

            for (int k = -1; k <= 1; k++) {
                if (rand % row == 0 && k == -1) continue;
                if (rand % row == row - 1 && k == 1) continue;
                if (this->blocks[rand + j * row + k].value < 9) {
                    this->blocks[rand + j * row + k].value++;
                }
            }
        }
    }

    for (auto i : blocks) {
        i.index = &i - &blocks[0];
    }
}

Board::~Board() {}

int Board::show_all_mine() {
    for (std::size_t i = 0; i < blocks.size(); ++i) {
        if (i % row == 0) {
            std::cout << std::endl;
        }

        if (blocks[i].value >= 9) {
            std::cout << "M ";
        } else {
            std::cout << blocks[i].value << " ";
        }
    }

    status = LOST;
    return 0;
}

int Board::timer() {
    auto startpoint = std::chrono::steady_clock::now();
    if (status == LOST || status == WON) {
        auto endpoint = std::chrono::steady_clock::now();
        std::chrono::steady_clock::duration elapsed = endpoint - startpoint;
        double elapsed_seconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)
                .count();
        std::cout << "End time: (" << elapsed_seconds << "s)\n";
    }
    return 0;
}

int Board::get_input() {
    int x, y;
    while (1) {
        std::cout << "Please enter the x and y coordinates of the block you "
                     "want to reveal: ";
        std::cin >> x >> y;
        if (x < 0 || x >= row || y < 0 ||
            y >= col) {
            std::cout << "Invalid input, please try again.\n";
            continue;
        }

        break;
    }

    return y * row + x;
}

int Board::start_game() {
    while (status == PLAYING) {
        system("clear");
        print_board();
        this->timer();
        block target_block = blocks[get_input()];
        reveal(target_block);

        if (this->n_revealed == row * col - n_mines) {
            status = WON;
            break;
        }
    }

    return 0;
}

int Board::print_board() {
    for (int i = 0; i < row * col; i++) {
        if (blocks[i].state == 0) {
            std::cout << "X ";
        } else if (blocks[i].state == 1) {
            std::cout << blocks[i].value << " ";
        } else {
            std::cout << "F ";
        }

        if (i % row == row - 1) {
            std::cout << std::endl;
        }
    }

    return 0;
}

int Board::reveal(block input) {
    if (input.value >= 9) {
        show_all_mine();
        this->status = LOST;
        return 0;
    }

    input.state = 1;
    this->n_revealed++;
    if (this->n_revealed == row * col - n_mines) {
        this->status = WON;
    }

    // fast reveal if the block is empty
    if (input.value == 0) {
        for (int i = -1; i <= 1; i++) {
            if (input.index < row && i == -1) continue;
            if (input.index >= row * (col - 1) && i == 1) continue;

            for (int j = -1; j <= 1; j++) {
                if (input.index % row == 0 && j == -1) continue;
                if (input.index % row == row - 1 && j == 1) continue;

                if (blocks[input.index + i * row + j].state == 0) {
                    reveal(blocks[input.index + i * row + j]);
                }
            }
        }
    }

    return 0;
}

int Board::flagged(std::size_t x, std::size_t y) {
    std::size_t i = y * row + x;
    blocks[i].state = 2;
    std::cout << "(" << x << ", " << y << ") flagged.\n";

    return 0;
}

int Board::remove_flagged(std::size_t x, std::size_t y) {
    std::size_t i = y * row + x;
    blocks[i].state = 0;
    std::cout << "(" << x << ", " << y << ")'s flag has been removed.\n";

    return 0;
}

int Board::flag_counter(int n_mines) {
    int flag = n_mines;
    for (std::size_t i = 0; i < blocks.size(); ++i) {
        if (blocks[i].state == 2) {
            flag--;
        } else if (blocks[i].state == 0) {
            flag++;
        }
    }

    return 0;
}

int Board::gl_init_board() {
    for (int i=0; i<row*col; i++) {
        double currnent_gl_x = ((i % row) + 0.1f) * 2.0f / row - 1.0f;
        blocks[i].gl_x = currnent_gl_x;
        blocks[i].gl_y = ((static_cast<float>(i) / row) + 0.1f) * 2.0f / col - 1.0f;
    }

    bool mouse_button_pressed = false;
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            mouse_button_pressed = true;
        } else if (mouse_button_pressed && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            gl_reveal(gl_get_block(window, xpos, ypos));
        }

        glfwPollEvents();
        gl_draw_board(window);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

int Board::gl_setup_block(block b, unsigned int& VBO, unsigned int& VAO, unsigned int& EBO) {
    double vertices[] = {b.gl_x,          b.gl_y,          0.0f,
                         b.gl_x + b.size, b.gl_y,          0.0f,
                         b.gl_x + b.size, b.gl_y + b.size, 0.0f,
                         b.gl_x,          b.gl_y + b.size, 0.0f};

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    return 0;
}

int Board::gl_draw_board(GLFWwindow* window) {
    for (int i = 0; i < row * col; i++) {
        unsigned int VBO, VAO, EBO;
        gl_setup_block(blocks[i], VBO, VAO, EBO);
        gl_draw_block(VAO, blocks[i]);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    return 0;
}

int Board::gl_draw_block(unsigned int VAO, block b) {
    if (bomb_count_color_map.find(b.value) == bomb_count_color_map.end()) {
        std::cerr << "錯誤: b.value: " << b.value << " 不在 bomb_count_color_map 的範圍內" << std::endl;
        return -1;
    }

    std::vector<float> rgb = bomb_count_color_map[b.value];
    glBindVertexArray(VAO);
    glColor3f(rgb[0], rgb[1], rgb[2]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    return 0;
}

block Board::gl_get_block(GLFWwindow* window, double x, double y) {
    block target_block;
    target_block.gl_x = (x + 1) * row / 2;
    target_block.gl_y = (y + 1) * col / 2;
    return target_block;
}

int Board::gl_reveal(block target_block) {
    // if the block is already revealed or flagged, do nothing
    if (target_block.state != 0) {
        return 0;
    }

    std::cout << "Reveal (" << target_block.gl_x << ", " << target_block.gl_y << ")\n";
    reveal(target_block);

    return 0;
}

int Board::gl_show_all_mine() {
    for (auto i : blocks) {
        if (i.value >= 9) {
            i.state = 1;
        }
    }

    gl_draw_board(glfwGetCurrentContext());
    return 0;
}

int Board::gl_main_menu() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 800, "Minesweeper", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_QUADS);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(-0.5f, 0.5f);
        glColor3f(0.0f, 1.0f, 2.0f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
