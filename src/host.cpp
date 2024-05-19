#include "host.h"
#include "network.h"

int host_game(u_int16_t port, int max_member) {
    max_member -= 1;  // exclude host

    // AF_INET: use IPv4
    // SOCK_DGRAM: UDP, SOCK_STREAM: TCP
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_address;

    if (socket_fd < 0) {
        return SOCKET_CREATE_FAILED;
    }

    // server connection information
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;  // vaild address accept
    server_address.sin_port = htons(port);        // use specify (include) port

    // bind port on host
    if (bind(socket_fd, (const struct sockaddr *)&server_address,
             sizeof(server_address)) < 0) {
        close(socket_fd);
        return BIND_FAILED;
    }

    // listen init
    if (listen(socket_fd, max_member) == SOCKET_CREATE_FAILED) {
        close(socket_fd);
        return LISTEN_ERROR;
    }

    if (close(socket_fd) < 0) {
        return SOCKET_CLOSE_ERROR;
    }

    // client connection information
    int reply_sockfd;
    sockaddr_in client_address;
    int client_index = 0;
    unsigned int client_len = sizeof(client_address);

    GL gl;
    bool game_started = false;

    while (!game_started) {
        reply_sockfd =
            accept(socket_fd, (struct sockaddr *)&client_address, &client_len);
    }

    // init board
    Board game(8, 8, 10);
    gl.init_board(game);
    // send board information to client
    send(socket_fd, &game, sizeof(game), 0);
    // need consider client accept later disconnect

    // 遊戲互動環節
    game_data data;
    std::thread(game_interaction, socket_fd, &data, &game).detach();

    if (close(socket_fd) < 0) {
        return SOCKET_CLOSE_ERROR;
    }

    return SUCESS;
}