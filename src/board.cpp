#include "board.h"

#include <random>
#include <iostream>
#include <chrono>

Board::Board(int row, int col, int n_mines):
       row(row),
       col(col),
       n_mines(n_mines) {
    blocks.resize(row * col);
    this->col = col;
    this->row = row;
    // Generate Mines Locations

    std::random_device rd;  // 取得隨機數種子
    std::mt19937 gen(rd()); // 使用 Mersenne Twister 引擎
    std::uniform_int_distribution<int> dis(0, this->row * this->col - 1);
    for (int i=0; i<this->n_mines; i++) {
        int rand = dis(gen);
        if (this->blocks[rand].value == 9) {
            i--;
            continue;
        }

        this->blocks[rand].value = 9;
        for (int j=-1; j<=1; j++) {
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
    for (int i=0; i<row * col; i++) {
        auto temp_x = i%col;
        auto temp_y = i/col;
        this->blocks[i].x = temp_x*this->blocks[i].size + (temp_x)*this->border;
        this->blocks[i].y = temp_y*this->blocks[i].size + (temp_y)*this->border;
    }
}

Board::~Board() {}

int Board::show_all_mine() {
    for (size_t i = 0; i < blocks.size(); ++i) {
        if (blocks[i].value == 9 && blocks[i].state != 1) {
            blocks[i].state = 1;
            std::cout << "Revealed mine at position: (" << blocks[i].x << ", " << blocks[i].y << ")\n";
        }
        status = LOST;
    }
    return 0;
}

int Board::timer() {
    auto startpoint = std::chrono::steady_clock::now();
    if (status == LOST || status == WON) {
        auto endpoint = std::chrono::steady_clock::now();
        std::chrono::steady_clock::duration elapsed = endpoint - startpoint;
        double elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        std::cout << "End time: (" << elapsed_seconds << "s)\n";
    }
    return 0;
}

std::pair<int, int> Board::get_input() {
    std::pair<int, int> result;
    while (1) {
        std::cout << "Please enter the x and y coordinates of the block you want to reveal: ";
        std::cin >> result.first >> result.second;
        if (result.first < 0 || result.first >= row || result.second < 0 || result.second >= col) {
            std::cout << "Invalid input, please try again.\n";
            continue;
        }

        break;
    }

    return result;
}

int Board::start_game() {
    while (status == PLAYING) {
        print_board();
        this->timer();
        std::pair<int, int> input = get_input();
        int index = input.first + input.second * row;
        if (blocks[index].value == 9) {
            show_all_mine();
            break;
        }

        blocks[index].state = 1;
        n_revealed++;
        if (this->n_revealed == row * col - n_mines) {
            status = WON;
            break;
        }
    }

    return 0;
}

int Board::print_board() {
    for (int i=0; i<row*col; i++) {
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

int Board::flagged(size_t x, size_t y){
    size_t i =  y * row + x;
    blocks[i].state = 2;
    std::cout<<"("<<blocks[i].x<<", "<<blocks[i].y<<") flagged.\n";

    return 0;
}

int Board::remove_flagged(size_t x, size_t y){
    size_t i =  y * row + x;
    blocks[i].state = 0;
    std::cout<<"("<<blocks[i].x<<", "<<blocks[i].y<<")'s flag has been removed.\n";

    return 0;
}