#ifndef RSO_SERVER_CLIENT_SERVER_H
#define RSO_SERVER_CLIENT_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 2121

struct server_t {
    int socket_fd;
    socklen_t server_socklen;
    struct sockaddr_in server_address;
};

extern struct server_t server;

#endif //RSO_SERVER_CLIENT_SERVER_H
