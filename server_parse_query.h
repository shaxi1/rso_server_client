#ifndef RSO_SERVER_CLIENT_SERVER_PARSE_QUERY_H
#define RSO_SERVER_CLIENT_SERVER_PARSE_QUERY_H

#include <stdbool.h>

#include "communication_common.h"

bool is_query_valid(struct message_t);
void determine_and_set_query_type(struct message_t *);
double get_number_from_message(struct message_t);

#endif //RSO_SERVER_CLIENT_SERVER_PARSE_QUERY_H
