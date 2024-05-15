#include "network.h"

#include <unistd.h>

int host_game(u_int16_t port, int max_member) {
    max_member -= 1;  // exclude host
    enum host_game_status {
        SUCCESS = 0,
        SOCKET_CREATE_FAILED = -1,
        BIND_FAILED = -2,
        LISTEN_ERROR = -3,
        SOCKET_CLOSE_ERROR = -4,
    };

    // AF_INET: use IPv4
    // SOCK_DGRAM: UDP, SOCK_STREAM: TCP
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_address;

    if (socket_fd < 0) {
        return socket_fd;
        // if failed is -1
    }

    // server connection information
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // vaild address accept
    server_address.sin_port = htons(port); // use specify (include) port

    // bind port on host
    if (bind(socket_fd, (const struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
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
    // 留著預備用
    /*
    struct client_info {
        int sockfd;
        struct client_address;
    };
    */

    pthread_t clients[max_member];
    pthread_t tid;

    bool game_status = true;

    while (game_status) {
        reply_sockfd = accept(socket_fd, (struct sockaddr *)&client_address, &client_len);

        // 遊戲互動環節
        // 接收: recv(int sockfd, void *buf, size_t len, int flags)
        // -> recv(socket_fd, RECV_MESSENGE, sizeof(RECV_MESSENGE), 0);
        // 傳送: send(int sockfd, const void *buf, size_t len, int flags)
        // -> send(socket_fd, MESSENGE, sizeof(MESSENGE), 0);
        // flags填入0即可

    }

    if (close(socket_fd) < 0) {
        return SOCKET_CLOSE_ERROR;
    }

    return 0;
}

int join_game(uint32_t host_address, uint16_t host_port) {
    enum join_game_status {
        SUCCESS = 0,
        SOCKET_CREATE_FAILED = -1,
        CONNECT_FAILED = -2, // (May server not listen)
        EMPTY = -3,
        SOCKET_CLOSE_ERROR = -4,
        MESSENGE_SEND_ERROR = -5,
    };

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
    if (connect(socket_fd, (struct sockaddr *)&server_address, server_len) == SOCKET_CREATE_FAILED) {
        close(socket_fd);
        return CONNECT_FAILED;
    }

    bool game_status = true;

    while (game_status) {

        // 遊戲互動環節
        // 接收: recv(int sockfd, void *buf, size_t len, int flags)
        // -> recv(socket_fd, RECV_MESSENGE, sizeof(RECV_MESSENGE), 0);
        // 傳送: send(int sockfd, const void *buf, size_t len, int flags)
        // -> send(socket_fd, MESSENGE, sizeof(MESSENGE), 0);
        // flags填入0即可

    }

    if (close(socket_fd) < 0) {
        return SOCKET_CLOSE_ERROR;
    }

    return 0;
}