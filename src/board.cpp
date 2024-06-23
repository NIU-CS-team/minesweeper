#include "board.h"

#include <iostream>
#include <random>

Board::Board(int row, int col, int n_mines)
    : row(row), col(col), n_mines(n_mines) {
    blocks.resize(row * col);
    this->col = col;
    this->row = row;

    for (auto& i : blocks) {
        i.index = &i - &blocks[0];
    }

    start_time = std::chrono::system_clock::now();
}

Board::~Board() {}

int Board::generate_mines() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, this->row * this->col - 1);
    for (int i = 0; i < this->n_mines; i++) {
        int rand = dis(gen);
        if (this->blocks[rand].value >= MINE) {
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
                if (this->blocks[rand + j * row + k].value < MINE) {
                    this->blocks[rand + j * row + k].value++;
                }
            }
        }
    }
    return 0;
}

int Board::clear() {
    for (auto& i : blocks) {
        i.value = 0;
        i.state = HIDDEN;
    }

    return 0;
}

int Board::show_all_mine() {
    system("clear");
    end_time = std::chrono::system_clock::now();
    for (std::size_t i = 0; i < blocks.size(); ++i) {
        if (i % row == 0) {
            std::cout << std::endl;
        }

        if (blocks[i].value >= MINE) {
            std::cout << "M ";
        } else {
            std::cout << blocks[i].value << " ";
        }
    }

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time);
    std::cout << "\nEnd time: (" << duration.count() / 1000000 << "s)\n";
    return 0;
}

int Board::get_input() {
    int x, y;
    char action;

    while (1) {
        std::cout << "Please enter r or f you want to reveal or flag call: ";
        std::cin >> action;
        if (action == 'r') {
            std::cout
                << "Please enter the x and y coordinates of the block you "
                   "want to reveal: ";
            std::cin >> x >> y;
            if (x < 0 || x >= row || y < 0 || y >= col) {
                std::cout << "Invalid input, please try again.\n";
                continue;
            }
            reveal(blocks[y * row + x]);
            break;
        }
        if (action == 'f') {
            std::cout
                << "Please enter the x and y coordinates of the block you "
                   "want to flag call: ";
            std::cin >> x >> y;
            if (x < 0 || x >= row || y < 0 || y >= col) {
                std::cout << "Invalid input, please try again.\n";
                continue;
            }
            if (blocks[y * row + x].state == FLAGGED) {
                remove_flagged(blocks[y * row + x]);
            } else {
                flagged(blocks[y * row + x]);
            }
            break;
        }
        std::cout << "Invalid input, please try again.\n";
    }

    return 0;
}

int Board::start_game() {
    generate_mines();
    while (this->status == PLAYING) {
        system("clear");
        print_board();
        get_input();
        check_win();
    }

    return 0;
}

int Board::print_board() {
    for (int i = 0; i < row * col; i++) {
        if (blocks[i].state == HIDDEN) {
            std::cout << "X ";
        } else if (blocks[i].state == REVEALED) {
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

int Board::reveal(block& target_block) {
    if (target_block.state == FLAGGED) {
        return 0;
    }

    if (first_move){
        if (target_block.value >= MINE) {
            generate_mines();
            start_time = std::chrono::system_clock::now();
            reveal(target_block);
            return 0;
        } else {
            first_move = false;
        }
    }

    if (target_block.value >= MINE) {
        this->status = LOST;
        show_all_mine();

        return 0;
    }

    bool fast = target_block.state == REVEALED;
    if (!fast) {
        target_block.state = REVEALED;
        this->n_revealed++;
        if (target_block.value != EMPTY) return 0;
    }


    int n_flagged = 0;
    for (int i = -1; i <= 1; i++) {
        if (target_block.index < this->row && i == -1) continue;
        if (target_block.index >= this->row * (this->col - 1) && i == 1)
            continue;

        for (int j = -1; j <= 1; j++) {
            if (target_block.index % this->row == 0 && j == -1) continue;
            if (target_block.index % this->row == this->row - 1 && j == 1)
                continue;
            if (i == 0 && j == 0) continue;

            block& block = this->blocks[target_block.index + i * this->row + j];
            if(!fast) {
                reveal(block);
            } else if(block.state == FLAGGED) {
                n_flagged++;
            }
        }
    }
    if (fast && n_flagged == target_block.value) {
        for (int i = -1; i <= 1; i++) {
            if (target_block.index < this->row && i == -1) continue;
            if (target_block.index >= this->row * (this->col - 1) && i == 1)
                continue;

            for (int j = -1; j <= 1; j++) {
                if (target_block.index % this->row == 0 && j == -1) continue;
                if (target_block.index % this->row == this->row - 1 && j == 1)
                    continue;
                if (i == 0 && j == 0) continue;

                block& block = this->blocks[target_block.index + i * this->row + j];
                if (block.state == HIDDEN) {
                    reveal(block);
                }
            }
        }
    }

    return 0;
}

int Board::flagged(block& target_block) {
    this->n_flags++;
    blocks[target_block.index].state = FLAGGED;
    return 0;
}

int Board::remove_flagged(block& target_block) {
    this->n_flags--;
    blocks[target_block.index].state = HIDDEN;
    return 0;
}

int Board::check_win() {
    if (this->n_revealed == row * col - n_mines) {  // win condition
        end_time = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time);
        std::cout << "End time: (" << duration.count() / 1000000 << "."
                  << duration.count() % 1000000 << "s)\n";
        status = WON;
    }

    return 0;
}
