#ifndef BLOCK_H
#define BLOCK_H

enum block_state { HIDDEN, REVEALED, FLAGGED };
enum block_value { EMPTY, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, MINE };

struct block {
    block_state state = HIDDEN;
    int value = EMPTY;  // 0: empty, 1-8: number of mines around, 9: mine
    double gl_x;
    double gl_y;
    int index;
};

#endif