#ifndef RSO_SERVER_CLIENT_SERVER_PARSE_AND_HANDLE_QUERY_H
#define RSO_SERVER_CLIENT_SERVER_PARSE_AND_HANDLE_QUERY_H

#include <stdbool.h>

#include "communication_common.h"

bool is_query_valid(struct message_t);
enum rq_t get_query_type(struct message_t);


#endif //RSO_SERVER_CLIENT_SERVER_PARSE_AND_HANDLE_QUERY_H
