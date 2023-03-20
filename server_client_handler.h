#ifndef RSO_SERVER_CLIENT_SERVER_CLIENT_HANDLER_H
#define RSO_SERVER_CLIENT_SERVER_CLIENT_HANDLER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "communication_common.h"
#include "server_parse_query.h"

extern atomic_int terminate_server;

struct client_t {
    int socket_fd;
    struct sockaddr_in client_address;
    socklen_t client_socklen;
    int taken;
    int client_idx;
    pthread_t thread;
};

struct client_list_t {
    int capacity;
    int clients_connected;
    struct client_t *clients;
};

extern struct client_list_t *client_list;

int initialize_server();
void *server_listen(void *arg);
int destroy_server();

int remove_client(int client_idx);
void *handle_client(void *arg);

char *get_server_date();

#endif //RSO_SERVER_CLIENT_SERVER_CLIENT_HANDLER_H
