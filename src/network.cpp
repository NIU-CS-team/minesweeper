#include "network.h"

#include <unistd.h>

#include <cstring>
#include <vector>
#include <mutex>

#include "gl.h"

std::mutex g_mutex;

int recv_data(int &socket_fd, char *buffer, game_data *data, Board *board) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (recv(socket_fd, buffer, sizeof(game_data), 0) <= 0) {
            close(socket_fd);
            return MESSENGE_RECV_ERROR;
    }

        std::memcpy(data, buffer, sizeof(game_data));
        if (data->action == REVEAL) {
            board->reveal(board->blocks[data->block_index]);
        } else if (data->action == FLAG) {
            board->flagged(board->blocks[data->block_index]);
        } else if (data->action == REMOVE_FLAG) {
            board->remove_flagged(board->blocks[data->block_index]);
        }
        memset(buffer, 0, sizeof(game_data));
        return SUCESS;
}

int game_interaction(int &socket_fd, game_data *data, Board *board) {
    while (data->game_status) {
        char buffer[sizeof(game_data)];

        std::thread(recv_data, std::ref(socket_fd), buffer, data, board).detach();
        // 預留Local interaction
        send(socket_fd, buffer, sizeof(game_data), 0);
    }

    return SUCESS;
}