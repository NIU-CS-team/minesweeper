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
        if (this->blocks[rand].value == 9) {
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
                this->blocks[rand + j * row + k].value++;
            }
        }
    }

    this->blocks.resize(row * col);
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

std::pair<int, int> Board::get_input() {
    std::pair<int, int> result;
    while (1) {
        std::cout << "Please enter the x and y coordinates of the block you "
                     "want to reveal: ";
        std::cin >> result.first >> result.second;
        if (result.first < 0 || result.first >= row || result.second < 0 ||
            result.second >= col) {
            std::cout << "Invalid input, please try again.\n";
            continue;
        }

        break;
    }

    return result;
}

int Board::start_game() {
    while (status == PLAYING) {
        system("clear");
        print_board();
        this->timer();
        reveal(get_input());

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

int Board::reveal(std::pair<int, int> input) {
    std::size_t index = input.second * row + input.first;
    if (blocks[index].value >= 9) {
        show_all_mine();
        this->status = LOST;
        return 0;
    }

    blocks[index].state = 1;
    this->n_revealed++;
    if (this->n_revealed == row * col - n_mines) {
        this->status = WON;
    }

    // fast reveal if the block is empty
    if (blocks[index].value == 0) {
        for (int i = -1; i <= 1; i++) {
            if (input.second < row && i == -1) continue;
            if (input.second >= row * (col - 1) && i == 1) continue;

            for (int j = -1; j <= 1; j++) {
                if (input.first % row == 0 && j == -1) continue;
                if (input.first % row == row - 1 && j == 1) continue;
                if (blocks[index + i * row + j].state == 0) {
                    system("clear");
                    reveal(std::make_pair(input.first + j, input.second + i));
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
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 800, "Minesweeper", NULL, NULL);
    if (window == NULL) {
        std::cerr << "無法建立 GLFW 視窗" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "無法初始化 GLEW" << std::endl;
        return -1;
    }

    for (int i=0; i<row*col; i++) {
        blocks[i].gl_x = ((i % row) + 0.1f) * 2.0f / row - 1.0f;
        blocks[i].gl_y = ((i / row) + 0.1f) * 2.0f / col - 1.0f;
    }

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            gl_reveal(window, xpos, ypos);
        }

        glfwPollEvents();
        gl_draw_board(window);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

int Board::gl_draw_board(GLFWwindow* window) {
    for (int i = 0; i < row * col; i++) {
        gl_draw_block(window, blocks[i]);
    }

    return 0;
}

int Board::gl_draw_block(GLFWwindow* window, block b) {
    double vertices[] = {b.gl_x,          b.gl_y,          0.0f,
                         b.gl_x + b.size, b.gl_y,          0.0f,
                         b.gl_x + b.size, b.gl_y + b.size, 0.0f,
                         b.gl_x,          b.gl_y + b.size, 0.0f};

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    if (b.state == 0) {
        glColor3f(1.0f, 1.0f, 1.0f); // 如果方塊未被揭示，設定顏色為白色
    } else if (b.state == 1) {
        glColor3f(0.5f, 0.5f, 0.5f); // 如果方塊被揭示，設定顏色為灰色
    } else {
        glColor3f(1.0f, 0.0f, 0.0f); // 如果方塊被標記為地雷，設定顏色為紅色
    }

    // draw the block by value
    if (b.value == 0) {
        glColor3f(0.0f, 0.0f, 0.0f);
    } else if (b.value == 1) {
        glColor3f(0.0f, 0.0f, 1.0f);
    } else if (b.value == 2) {
        glColor3f(0.0f, 1.0f, 0.0f);
    } else if (b.value == 3) {
        glColor3f(1.0f, 0.0f, 0.0f);
    } else if (b.value == 4) {
        glColor3f(0.0f, 0.0f, 1.0f);
    } else if (b.value == 5) {
        glColor3f(0.0f, 1.0f, 1.0f);
    } else if (b.value == 6) {
        glColor3f(1.0f, 1.0f, 0.0f);
    } else if (b.value == 7) {
        glColor3f(0.5f, 0.5f, 0.5f);
    } else if (b.value == 8) {
        glColor3f(0.5f, 0.5f, 0.5f);
    } else {
        glColor3f(1.0f, 0.0f, 0.0f);
    }

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    return 0;
}

int Board::gl_reveal(GLFWwindow* window, double x, double y) {
    std::pair<int, int> target_block;
    target_block = std::make_pair((x + 1) * row / 2, (y + 1) * col / 2);
    std::cout << "Reveal (" << target_block.first << ", " << target_block.second << ")\n";
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