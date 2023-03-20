#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <stdatomic.h>
#include <errno.h>

#include "server_client_handler.h"
#include "server.h"
#include "server_parse_query.h"

struct server_t server;
struct client_t client;
struct query_list_t query_list;
time_t timer;
atomic_int terminate_server = 0;

int initialize_server()
{
    /* init timer */
    timer = time(NULL);

    server.socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server.socket_fd < 0)
        return printf("Error while creating socket\n"), 1;

    server.server_address.sin_family = AF_INET;
    server.server_address.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server.server_address.sin_port = htons(SERVER_PORT);
    server.server_socklen = sizeof(server.server_address);

//    setsockopt(server.socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    int r = bind(server.socket_fd, (struct sockaddr *) &server.server_address, server.server_socklen);
    if (r < 0)
        return printf("Error while binding socket\n"), 2;

    query_list.queries = calloc(sizeof(struct query_t), STARTING_QUERY_LIST_CAPACITY);
    if (query_list.queries == NULL)
        return printf("Error while allocating memory for query list\n"), 3;

    query_list.capacity = STARTING_QUERY_LIST_CAPACITY;

    return 0;
}

void add_client(struct sockaddr_in *temp_client_address, socklen_t temp_client_socklen, int temp_socket_fd) {
    client.socket_fd = temp_socket_fd;
    client.client_address = *temp_client_address;
    client.client_socklen = temp_client_socklen;
}

void *server_listen(void *arg)
{
    int r = listen(server.socket_fd, 5);
    if (r < 0)
        return NULL;

    struct sockaddr_in temp_client_address;
    socklen_t temp_client_socklen = sizeof(temp_client_address);
    printf("Waiting for client...\n");
    while (1) {
        atomic_int check = atomic_load(&terminate_server);
        if (check == 1)
            break;

        int temp_socket_fd = accept(server.socket_fd, (struct sockaddr *) &temp_client_address, &temp_client_socklen);
        if (temp_socket_fd < 0)
            return NULL;

        printf("client connected\n");
        add_client(&temp_client_address, temp_client_socklen, temp_socket_fd);
        handle_client();
    }

    return NULL;
}

void add_query_to_list(struct query_t query)
{
    if (query_list.size == query_list.capacity) {
        query_list.capacity *= 2;
        struct query_t *temp = realloc(query_list.queries, query_list.capacity * sizeof(struct query_t));
        if (temp == NULL)
            return;
        query_list.queries = temp;
    }

    query_list.queries[query_list.size] = query;
    query_list.size++;
}

void execute_and_pop_query()
{
    if (query_list.size == 0)
        return;

    struct query_t query = query_list.queries[query_list.size - 1];
    query_list.size--;

    struct message_t reply;
    char payload[PAYLOAD_SIZE];
    memset(reply.payload, 0, PAYLOAD_SIZE);
    if (query.rq == SQUARE) {
        double result = sqrt(query.number);
        reply.rq = SQUARE;
        memcpy(payload, &result, sizeof(double));
        prep_response(&reply, query.rq, payload, sizeof(double));
    } else {
        char *date = get_server_date();
        sprintf(payload, "%s\n", date);
        reply.rq = DATE;
        prep_response(&reply, query.rq, payload, strlen(date));
        free(date);
    }

//    printf("Sending response to the client\n");
    write(client.socket_fd, &reply, sizeof(struct message_t));
}

void handle_client()
{
    while (1) {
        struct message_t message;
        read(client.socket_fd, &message, sizeof(struct message_t));

        determine_and_set_query_type(&message);

        struct query_t query;
        query.query_id = query_list.size;
        query.rq = message.rq;
        if (message.rq == SQUARE)
            query.number = get_number_from_message(message);

        add_query_to_list(query);
        execute_and_pop_query();
    }
}

char *get_server_date() {
    char ret[DATE_SIZE];
    struct tm *time_now = localtime(&timer);

    strftime(ret, DATE_SIZE, "%c", time_now);
    return strdup(ret);
}

int destroy_server()
{
    free(query_list.queries);
    close(client.socket_fd);
    close(server.socket_fd);

    return 0;
}