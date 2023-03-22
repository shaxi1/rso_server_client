#include <arpa/inet.h>

#include "convert.h"

void convert_to_big_endian(struct message_t *msg) {
    for (int i = 0; i < CODE_LENGTH; i++) {
        msg->code[i] = htonl(msg->code[i]);
    }
    msg->rq = htonl(msg->rq);
    for (int i = 0; i < PAYLOAD_SIZE; i++) {
        msg->payload[i] = htonl(msg->payload[i]);
    }
}

void convert_from_big_endian(struct message_t *msg) {
    for (int i = 0; i < CODE_LENGTH; i++) {
        msg->code[i] = ntohl(msg->code[i]);
    }
    msg->rq = ntohl(msg->rq);
    for (int i = 0; i < PAYLOAD_SIZE; i++) {
        msg->payload[i] = ntohl(msg->payload[i]);
    }
}

