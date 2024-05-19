#include "network.h"

#include <unistd.h>

#include <cstring>
#include <vector>

#include "gl.h"

int game_interaction(int socket_fd, game_data *data, Board *board) {
    // 接收: recv(int sockfd, void *buf, size_t len, int flags)
    // -> recv(socket_fd, RECV_MESSENGE, sizeof(RECV_MESSENGE), 0);
    // 傳送: send(int sockfd, const void *buf, size_t len, int flags)
    // -> send(socket_fd, MESSENGE, sizeof(MESSENGE), 0);
    // flags填入0即可
    while (data->game_status) {
        char buffer[sizeof(game_data)];

        if (recv(socket_fd, buffer, sizeof(buffer), 0) <= 0) {
            close(socket_fd);
            return -1;
        }

        std::memcpy(data, buffer, sizeof(game_data));

        // 遊戲互動環節
        if (data->action == REVEAL) {
            board->reveal(board->blocks[data->block_index]);
        } else if (data->action == FLAG) {
            board->flagged(board->blocks[data->block_index]);
        } else if (data->action == REMOVE_FLAG) {
            board->remove_flagged(board->blocks[data->block_index]);
        }
    }
    close(socket_fd);
    return 0;
}
