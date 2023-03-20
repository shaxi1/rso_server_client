#ifndef RSO_SERVER_CLIENT_CLIENT_H
#define RSO_SERVER_CLIENT_CLIENT_H

#include "server.h"
#include "communication_common.h"

void prep_request(struct message_t *message, enum rq_t rq, double number);
void *read_from_server(void *arg);


#endif //RSO_SERVER_CLIENT_CLIENT_H
