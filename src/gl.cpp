#include "gl.h"
#include "board.h"
#include "block.h"

#include <iostream>

#include <GL/glew.h>

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
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    return 0;
}

int GL::init_board(Board board) {
    for (int i=0; i<board.row*board.col; i++) {
        double currnent_gl_x = ((i % board.row) + 0.1f) * 2.0f / board.row - 1.0f;
        board.blocks[i].gl_x = currnent_gl_x;
        board.blocks[i].gl_y = ((static_cast<float>(i) / board.row) + 0.1f) * 2.0f / board.col - 1.0f;
    }

    return 0;
}

int GL::setup_block(block b) {
    double vertices[] = {b.gl_x,          b.gl_y,          0.0f,
                         b.gl_x + b.size, b.gl_y,          0.0f,
                         b.gl_x + b.size, b.gl_y + b.size, 0.0f,
                         b.gl_x,          b.gl_y + b.size, 0.0f};

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};
    glGenVertexArrays(1, &b.VAO);
    glGenBuffers(1, &b.VBO);
    glGenBuffers(1, &b.EBO);
    glBindVertexArray(b.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, b.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    return 0;
}

int GL::draw_board(Board board) {
    for (int i = 0; i < board.row * board.col; i++) {
        this->setup_block(board.blocks[i]);
        this->draw_block(board.blocks[i]);
    }

    return 0;
}

int GL::draw_block(block b) {
    if (bomb_count_color_map.find(b.value) == bomb_count_color_map.end()) {
        std::cerr << "錯誤: b.value: " << b.value << " 不在 bomb_count_color_map 的範圍內" << std::endl;
        return -1;
    }

    std::vector<float> rgb = bomb_count_color_map[b.value];
    glBindVertexArray(b.VAO);
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

    target_block.index = static_cast<int>((y + 1.0) / 2.0 * board.col) * board.row + static_cast<int>((x + 1.0) / 2.0 * board.row);
    return target_block;
}

int GL::reveal(Board board, block target_block) {
    // if the block is already revealed or flagged, do nothing
    if (target_block.state != 0) {
        return 0;
    }

    // reveal the block
    target_block.state = 1;

    // if the block is a mine, stop revealing
    if (target_block.value >= 9) {
        return 0;
    }

    // reveal surrounding blocks
    // (you need to implement this part according to your game logic)
    // for (block surrounding_block : get_surrounding_blocks(target_block)) {
    //     reveal(surrounding_block);
    // }

    if (target_block.value == 0) {
        for (int i = -1; i <= 1; i++) {
            reveal(board, board.blocks[(target_block.index + i) * board.row]);
        }

        reveal(board, board.blocks[target_block.index - 1]);
        reveal(board, board.blocks[target_block.index + 1]);

        for (int i = -1; i <= 1; i++) {
            reveal(board, board.blocks[(target_block.index + i) * board.row]);
        }
    }

    return 0;
}

int GL::show_all_mine(Board board) {
    for (auto i : board.blocks) {
        if (i.value >= 9) {
            i.state = 1;
        }
    }

    this->draw_board(board);
    return 0;
}

void GL::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Check if the mouse is inside the button
        if (xpos >= -0.5f && xpos <= 0.5f &&
            ypos >= 0.2f && ypos <= 0.5f) {
            // The first button was clicked
        }
        if (xpos >= -0.5f && xpos <= 0.5f &&
            ypos >= -0.1f && ypos <= 0.1f) {
            // The second button was clicked
        }
        if (xpos >= -0.5f && xpos <= 0.5f &&
            ypos >= -0.5f && ypos <= -0.2f) {
            // The third button was clicked
        }
    }
}

int GL::main_menu() {
    // wait for user choose host or join
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_QUADS);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(0.5f, 0.2f);
        glVertex2f(-0.5f, 0.2f);
        glVertex2f(-0.5f, 0.5f);
        glVertex2f(0.5f, 0.1f);
        glVertex2f(0.5f, -0.1f);
        glVertex2f(-0.5f, -0.1f);
        glVertex2f(-0.5f, 0.1f);
        glVertex2f(0.5f, -0.2f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(-0.5f, -0.2f);
        glColor3f(0.0f, 1.0f, 2.0f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // if user choose host, wait for another player to join
    // if user choose join, show ip address input box

    return 0;
}

int GL::play_single(Board board) {
    while (!glfwWindowShouldClose(window) && board.status == board.PLAYING) {
        draw_board(board);
        double xpos, ypos;  // 修改這裡
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