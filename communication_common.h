#ifndef RSO_SERVER_CLIENT_COMMUNICATION_COMMON_H
#define RSO_SERVER_CLIENT_COMMUNICATION_COMMON_H

#include <stdint.h>

#define DATE_SIZE 64
#define PAYLOAD_SIZE (sizeof(double) + DATE_SIZE)

#define QUERY_BIT 0
#define RESPONSE_BIT 3
#define CODE_LENGTH 8

enum rq_t {
    SQUARE = 1,
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
