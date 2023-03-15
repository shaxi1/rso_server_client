#ifndef RSO_SERVER_CLIENT_SERVER_CLIENT_HANDLER_H
#define RSO_SERVER_CLIENT_SERVER_CLIENT_HANDLER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "communication_common.h"
#include "server_parse_query.h"

struct client_t {
    int socket_fd;
    struct sockaddr_in client_address;
};

struct client_list_t {
    int capacity;
    int clients_connected;
    struct client_t *clients;
//    struct pthread_t
};

extern struct client_t *client_list;

void initialize_server();
void destroy_server();

void remove_client();
void add_client();


#endif //RSO_SERVER_CLIENT_SERVER_CLIENT_HANDLER_H
