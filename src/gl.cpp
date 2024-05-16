#include "gl.h"
#include "board.h"

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

int GL::init_board(std::vector<block> blocks, int row, int col, GLFWwindow* window) {
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
            this->reveal(this->get_block(window, xpos, ypos, row, col));
        }

        glfwPollEvents();
        this->draw_board(window, blocks, row, col);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

int GL::setup_block(block b, unsigned int& VBO, unsigned int& VAO, unsigned int& EBO) {
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

int GL::draw_board(GLFWwindow* window, std::vector<block> blocks, int row, int col) {
    for (int i = 0; i < row * col; i++) {
        unsigned int VBO, VAO, EBO;
        this->setup_block(blocks[i], VBO, VAO, EBO);
        this->draw_block(VAO, blocks[i]);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    return 0;
}

int GL::draw_block(unsigned int VAO, block b) {
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

block GL::get_block(GLFWwindow* window, double x, double y, int row, int col) {
    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);

    x = (x / window_width) * 2.0 - 1.0;
    y = ((window_height - y) / window_height) * 2.0 - 1.0;

    block target_block;
    target_block.index = static_cast<int>((y + 1.0) / 2.0 * col) * row + static_cast<int>((x + 1.0) / 2.0 * row);
    return target_block;
}

int GL::reveal(block target_block) {
    // if the block is already revealed or flagged, do nothing
    if (target_block.state != 0) {
        return 0;
    }

    std::cout << "Reveal (" << target_block.gl_x << ", " << target_block.gl_y << ")\n";
    reveal(target_block);

    return 0;
}

int GL::show_all_mine(std::vector<block> blocks, int row, int col) {
    for (auto i : blocks) {
        if (i.value >= 9) {
            i.state = 1;
        }
    }

    this->draw_board(glfwGetCurrentContext(), blocks, row, col);
    return 0;
}

int GL::main_menu() {
    // wait for user choose host or join
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

    // if user choose host, wait for another player to join
    // if user choose join, show ip address input box

    return 0;
}

int GL::play_single(Board board) {
    while (!glfwWindowShouldClose(window)) {
        draw_board(window, board.blocks, board.row, board.col);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}