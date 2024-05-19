#include "guest.h"

#include <cstring>

#include "network.h"

int join_game(uint32_t host_address, uint16_t host_port) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        return SOCKET_CREATE_FAILED;
    }

    // server information
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = host_address;
    server_address.sin_port = htons(host_port);
    int server_len = sizeof(server_address);

    // try connect to server
    if (connect(socket_fd, (struct sockaddr *)&server_address, server_len) ==
        SOCKET_CREATE_FAILED) {
        close(socket_fd);
        return CONNECT_FAILED;
    }

    Board board(8, 8, 10);
    char board_buffer[sizeof(Board)];
    ssize_t bytes_received =
        recv(socket_fd, board_buffer, sizeof(board_buffer), 0);
    if (bytes_received != sizeof(board_buffer)) {
        close(socket_fd);
        return MESSENGE_RECV_ERROR;
    }
    std::memcpy(&board, board_buffer, sizeof(Board));

    game_data data;
    std::thread(game_interaction, socket_fd, &data, &board).detach();

    if (close(socket_fd) < 0) {
        return SOCKET_CLOSE_ERROR;
    }

    return SUCESS;
}
