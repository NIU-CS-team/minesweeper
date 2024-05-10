#include "network.h"

#include <unistd.h>

int host_game(int port, int member) {
    // return code:
    //  0: success
    // -1: socket create failed
    // -2: bind failed
    // -3: listen error

    // AF_INET: use IPv4
    // SOCK_DGRAM: UDP, SOCK_STREAM: TCP
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in remote_address;

    if (socket_fd < 0) {
        return socket_fd;
        // if failed is -1
    }

    // connection information
    remote_address.sin_family = AF_INET;
    remote_address.sin_addr.s_addr = INADDR_ANY; // vaild address accept
    remote_address.sin_port = htons(port); // use specify (include) port

    // bind port on host
    if (bind(socket_fd, (const struct sockaddr *)&remote_address, sizeof(remote_address)) < 0) {
        close(socket_fd);
        return -2;
    }

    // listen init
    if(listen(socket_fd, member) == -1) {
        close(socket_fd);
        return -3;
    }

    return 0;
}