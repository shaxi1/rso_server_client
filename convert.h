#ifndef RSO_SERVER_CLIENT_CONVERT_H
#define RSO_SERVER_CLIENT_CONVERT_H

#include "communication_common.h"

void convert_to_big_endian(struct message_t *msg);
void convert_from_big_endian(struct message_t *msg);

#endif //RSO_SERVER_CLIENT_CONVERT_H
