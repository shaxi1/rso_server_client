#ifndef RSO_SERVER_CLIENT_SERVER_H
#define RSO_SERVER_CLIENT_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>

#include "communication_common.h"

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 2119
#define STARTING_QUERY_LIST_CAPACITY 10

struct server_t {
    int socket_fd;
    socklen_t server_socklen;
    struct sockaddr_in server_address;
};

struct query_t {
    int query_id;
    enum rq_t rq;
    double number;
};

struct query_list_t {
    struct query_t *queries;
    int capacity;
    int size;
};

extern struct server_t server;
extern struct query_list_t query_list;

#endif //RSO_SERVER_CLIENT_SERVER_H
