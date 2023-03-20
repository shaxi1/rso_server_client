#ifndef RSO_SERVER_CLIENT_SERVER_CLIENT_HANDLER_H
#define RSO_SERVER_CLIENT_SERVER_CLIENT_HANDLER_H

#include <sys/socket.h>
#include <arpa/inet.h>

#include "communication_common.h"
#include "server_parse_query.h"

struct client_t {
    int socket_fd;
    struct sockaddr_in client_address;
    socklen_t client_socklen;
};

extern struct client_t client;
extern atomic_int terminate_server;

int initialize_server();
void *server_listen(void *arg);
int destroy_server();

void handle_client();

char *get_server_date();

#endif //RSO_SERVER_CLIENT_SERVER_CLIENT_HANDLER_H
