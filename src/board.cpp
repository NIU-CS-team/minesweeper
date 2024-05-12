#include "board.h"

#include <chrono>
#include <iostream>
#include <random>

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


int Board::flagged(block target_block) {
    blocks[target_block.index].state = 2;
    return 0;
}

int Board::remove_flagged(block target_block) {
    blocks[target_block.index].state = 0;
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
