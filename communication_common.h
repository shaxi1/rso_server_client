#ifndef RSO_SERVER_CLIENT_COMMUNICATION_COMMON_H
#define RSO_SERVER_CLIENT_COMMUNICATION_COMMON_H

#include <stdint.h>

#define DATE_SIZE 64
#define PAYLOAD_SIZE (sizeof(double) + DATE_SIZE)

#define QUERY_BYTE 0
#define RESPONSE_BYTE 3
#define CODE_LENGTH 4

enum rq_t {
    SQUARE = 1,
    DATE
};

struct message_t {
    /* 0000 X00Y
     * X = 1 - response
     * Y = 1 - square rq
     * Y = 2 - date rq */
    char code[CODE_LENGTH];

    enum rq_t rq;
    char payload[PAYLOAD_SIZE];
};

#endif //RSO_SERVER_CLIENT_COMMUNICATION_COMMON_H
