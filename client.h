#ifndef RSO_SERVER_CLIENT_CLIENT_H
#define RSO_SERVER_CLIENT_CLIENT_H

#include "server.h"
#include "communication_common.h"

void prep_request(struct message_t *message, enum rq_t rq, void *payload, size_t payload_size);

void print_square_result(int socket_fd);
void print_date_response(int socket_fd);


#endif //RSO_SERVER_CLIENT_CLIENT_H
