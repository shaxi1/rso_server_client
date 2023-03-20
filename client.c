#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "client.h"

int main()
{
    struct sockaddr_in server_address;
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server_address.sin_port = htons(SERVER_PORT);

    printf("Specify query type: 1 - square, 2 - date): ");
    int query_type;
    scanf("%d", &query_type);
    if (query_type != 1 && query_type != 2)
        return printf("Invalid query type, quitting...\n"), 1;

    if (query_type == 1) {
        printf("Specify number to square: ");
        int number;
        scanf("%d", &number);
        if (number < 0)
            return printf("Invalid number, quitting...\n"), 2;

        struct message_t message;
        prep_request(&message, SQUARE, &number, sizeof(number));

        printf("Sending request to server...\n");
        send(socket_fd, &message, sizeof(message), 0);

        print_square_result(socket_fd);

    } else {
        struct message_t message;
        prep_request(&message, DATE, NULL, 0);

        printf("Sending request to server...\n");
        send(socket_fd, &message, sizeof(message), 0);

        print_date_response(socket_fd);
    }

    close(socket_fd);
    return 0;
}

void print_date_response(int socket_fd) {
    struct message_t response;
    recv(socket_fd, &response, sizeof(response), 0);

    char date[DATE_SIZE];
    memcpy(date, response.payload, DATE_SIZE);
    printf("Response received: %s\n", date);
}

void print_square_result(int socket_fd) {
    struct message_t response;
    recv(socket_fd, &response, sizeof(response), 0);

    double result;
    memcpy(&result, response.payload, sizeof(result));
    printf("Response received: %lf\n", result);
}

void prep_query_code(enum rq_t rq, struct message_t *message)
{
    if (rq != SQUARE && rq != DATE)
        return;

    if (rq == SQUARE)
        SET_BIT(message->code, 1);
    else
        SET_BIT(message->code, 2);
}

void prep_request(struct message_t *message, enum rq_t rq, void *payload, size_t payload_size)
{
    if (rq != SQUARE && rq != DATE)
        return;

    memset(message, 0, sizeof(struct message_t));
    message->rq = rq;
    if (payload != NULL)
        memcpy(message->payload, payload, payload_size);
}