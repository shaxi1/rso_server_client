#ifndef RSO_SERVER_CLIENT_COMMUNICATION_COMMON_H
#define RSO_SERVER_CLIENT_COMMUNICATION_COMMON_H

#include <stdint.h>

#define DATE_SIZE 20 // to determine https://stackoverflow.com/questions/1442116/how-to-get-the-date-and-time-values-in-a-c-program
#define PAYLOAD_SIZE (sizeof(double) + DATE_SIZE)

enum rq_t {
    SQUARE,
    DATE
};

struct message_t {
    /* 0000 X00Y
     * X = 1 - response
     * Y = 1 - square rq
     * Y = 2 - date rq */
    uint8_t code;

    enum rq_t rq;
    char payload[PAYLOAD_SIZE];
};

#endif //RSO_SERVER_CLIENT_COMMUNICATION_COMMON_H
