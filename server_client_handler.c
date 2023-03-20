#include <stdatomic.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#include "server_client_handler.h"
#include "server.h"
#include "server_parse_query.h"

#define STARTING_CLIENTS_CAPACITY 10

struct client_list_t *client_list;
struct server_t server;
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

    int r = bind(server.socket_fd, (struct sockaddr *) &server.server_address, server.server_socklen);
    if (r < 0)
        return printf("Error while binding socket\n"), 2;

    client_list = calloc(1, sizeof(struct client_list_t));
    if (client_list == NULL)
        return printf("Error while allocating memory for client list\n"), 3;
    client_list->clients = calloc(STARTING_CLIENTS_CAPACITY, sizeof(struct client_t));
    if (client_list->clients == NULL) {
        free(client_list);
        return printf("Error while allocating memory for client list\n"), 4;
    }

    client_list->capacity = STARTING_CLIENTS_CAPACITY;
    client_list->clients_connected = 0;

    return 0;
}

int server_increase_client_list_capacity()
{
    client_list->capacity *= 2;
    struct client_t *temp = realloc(client_list->clients, client_list->capacity * sizeof(struct client_t));
    if (temp == NULL)
        return 1;
    client_list->clients = temp;

    for (int i = client_list->capacity / 2; i < client_list->capacity; i++)
        client_list->clients[i].taken = 0;

    printf("Increased client list capacity to %d\n", client_list->capacity);

    return 0;
}

void add_client(struct sockaddr_in *temp_client_address, socklen_t temp_client_socklen, int temp_socket_fd) {
    int i = 0;
    for (; i < client_list->capacity; i++)
        if (client_list->clients[i].taken == 0)
            break;

    client_list->clients[i].socket_fd = temp_socket_fd;
    client_list->clients[i].client_address = (*temp_client_address);
    client_list->clients[i].client_socklen = temp_client_socklen;
    client_list->clients[i].taken = 1;
    client_list->clients[i].client_idx = i;
    client_list->clients_connected++;

    /* create thread for the specific client and wait for a query */
    pthread_create(&client_list->clients->thread, NULL, handle_client, (void *) &client_list->clients[i]);
}

void *server_listen(void *arg)
{
    int r = listen(server.socket_fd, 5);
    if (r < 0)
        return NULL;

    struct sockaddr_in temp_client_address;
    socklen_t temp_client_socklen = sizeof(temp_client_address);
    while (1) {
        atomic_int check = atomic_load(&terminate_server);
        if (check == 1)
            break;

        printf("Waiting for clients...\n");

        if (client_list->clients_connected == client_list->capacity)
            if (server_increase_client_list_capacity() != 0)
                return NULL;

        int temp_socket_fd = accept(server.socket_fd, (struct sockaddr *) &temp_client_address, &temp_client_socklen);
        if (temp_socket_fd < 0)
            return NULL;
        add_client(&temp_client_address, temp_client_socklen, temp_socket_fd);
    }

    return NULL;
}

void *handle_client(void *arg)
{
    struct client_t *client = (struct client_t *) arg;

    struct message_t message;
    read(client->socket_fd, &message, sizeof(struct message_t));

    if (!is_query_valid(message)) {
        printf("Invalid query, client idx:%d\n", client->client_idx);
        remove_client(client->client_idx);
        return NULL;
    }

    determine_and_set_query_type(&message);

    struct message_t reply;
    memset(&reply, 0, sizeof(struct message_t));
    if (message.rq == SQUARE) {
        double num_to_square = get_number_from_message(message);
        num_to_square *= sqrt(num_to_square);
        prep_response(&reply, SQUARE, (void *)&num_to_square, sizeof(double));
    } else if (message.rq == DATE) {
        char *date = get_server_date();
        prep_response(&reply, DATE, (void *)date, strlen(date) + 1);
        free(date);
    }

    write(client->socket_fd, &reply, sizeof(struct message_t));
    remove_client(client->client_idx);

    return NULL;
}

char *get_server_date() {
    char ret[DATE_SIZE];
    struct tm *time_now = localtime(&timer);

    strftime(ret, DATE_SIZE, "%c", time_now);
    return strdup(ret);
}

int destroy_server()
{
    if (client_list == NULL && client_list->clients == NULL)
        return printf("Error while destroying server\n"), 1;

    for (int i = 0; i < client_list->capacity; i++)
        if (client_list->clients[i].taken == 1)
            remove_client(i);

    for (int i = 0; i < client_list->capacity; i++)
        if (client_list->clients[i].taken == 1)
            pthread_join(client_list->clients[i].thread, NULL);

    close(server.socket_fd);
    free(client_list->clients);
    free(client_list);

    return 0;
}

int remove_client(int client_idx)
{
    if (client_idx < 0 || client_idx >= client_list->capacity)
        return printf("Wrong params\n"), 1;
    if (client_list->clients[client_idx].taken == 0)
        return printf("Wrong params\n"), 1;

    close(client_list->clients[client_idx].socket_fd);
    client_list->clients[client_idx].taken = 0;
    client_list->clients_connected--;

    return 0;
}