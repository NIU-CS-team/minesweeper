#ifndef BLOCK_H
#define BLOCK_H

struct block {
    const float size = 0.02f;
    int state = 0;  // 0: hidden, 1: revealed, 2: flagged
    int value = 0;  // 0: empty, 1-8: number of mines around, 9: mine
    double gl_x = 0;
    double gl_y = 0;
    int index = 0;

    unsigned int VAO, VBO, EBO;
};

#endif