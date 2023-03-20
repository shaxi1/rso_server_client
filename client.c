#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "client.h"


int main()
{
    int socket_fd;
    struct sockaddr_in server_address;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("socket");
        return 1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    if (connect(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
        return printf("Connection failed\n"), 1;

    pthread_t t;
    pthread_create(&t, NULL, read_from_server, &socket_fd);

    printf("Connected to server\n");
    printf("s square, d date, q quit\n");
    while (1) {
        struct message_t message;
        char c;
        scanf("%c%*c", &c);
        if (c == 'q') {
            printf("Quitting...\n");
            break;
        } else if (c == 's') {
            int number;
            printf("Enter number to square: ");
            scanf("%d%*c", &number);
            prep_request(&message, SQUARE, number);

        } else if (c == 'd') {
            printf("Sending date request...\n");
            prep_request(&message, DATE, 0);
        } else {
            printf("Unknown command\n");
        }

        write(socket_fd, &message, sizeof(struct message_t));
    }

    return 0;
}

void prep_request(struct message_t *message, enum rq_t rq, double number)
{
    memset(message, 0, sizeof(struct message_t));
    message->rq = rq;
    if (rq == SQUARE) {
        memcpy(message->payload, &number, sizeof(double));
        message->code[0] = 1;
    } else if (rq == DATE) {
        message->code[0] = 2;
    }
}

void *read_from_server(void *arg)
{
    struct message_t message;
    int socket_fd = *(int *) arg;
    read(socket_fd, &message, sizeof(struct message_t));

    if (message.rq == SQUARE) {
        double result;
        memcpy(&result, message.payload, sizeof(double));
        printf("Result: %f\n", result);
    } else if (message.rq == DATE) {
        printf("Date: %s\n", message.payload);
    }

    return NULL;
}


