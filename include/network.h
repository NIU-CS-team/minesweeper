#ifndef NETWORK_H
#define NETWORK_H
#if defined(_WIN32)
    #define _WINSOCK_DEPRECATED_NO_WARNINGS
    #include<winsock2.h>
    #include <Ws2tcpip.h>

#else
    #include <arpa/inet.h>
    #include <sys/socket.h>
    
#endif

#endif