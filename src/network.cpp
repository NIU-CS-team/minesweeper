#include "network.h"

#include <unistd.h>

#include <cstring>
#include <vector>
#include <mutex>
#include <thread>

#include "board.h"

std::mutex g_mutex;
int Network::recv_data(int &socket_fd, char *buffer,
                       std::pair<game_action, int> *data) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (recv(socket_fd, buffer, sizeof(data), 0) <= 0) {
        close(socket_fd);
        return MESSENGE_RECV_ERROR;
    }

    std::memcpy(data, buffer, sizeof(*data));
    if (data->first == REVEAL) {
        reveal(blocks[data->second]);
    } else if (data->first == FLAG) {
        flagged(blocks[data->second]);
    } else if (data->first == REMOVE_FLAG) {
        remove_flagged(blocks[data->second]);
    }
    memset(buffer, 0, sizeof(data));
    return SUCESS;
}
