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

    int flag = 0;
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
            prep_request(&message, DATE, 0);
        } else {
            printf("Unknown command\n");
        }

        printf("Sending request...\n");
        write(socket_fd, &message, sizeof(struct message_t));
        printf("Request sent\n");

        if (flag == 0) {
            pthread_t thread;
            pthread_create(&thread, NULL, read_from_server, &socket_fd);
            flag = 1;
        }
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
    while (1) {
        read(socket_fd, &message, sizeof(struct message_t));

        if (message.rq == SQUARE) {
            double result;
            memcpy(&result, message.payload, sizeof(double));
            printf("Result: %lf\n", result);
        } else if (message.rq == DATE) {
            printf("Date: %s\n", message.payload);
        }
        fflush(stdout);
    }

    return NULL;
}


