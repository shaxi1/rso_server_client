#include <string.h>

#include "server_parse_query.h"

int get_query_byte(struct message_t message)
{
    return message.code[QUERY_BYTE];
}

int get_response_byte(struct message_t message)
{
    return message.code[RESPONSE_BYTE];
}

void set_response_byte(struct message_t *message, int value)
{
    message->code[RESPONSE_BYTE] = value;
}

void set_query_byte(struct message_t *message, enum rq_t rq)
{
    if (rq != SQUARE && rq != DATE)
        return;

    if (rq == SQUARE)
        message->code[QUERY_BYTE] = 1;
    else
        message->code[QUERY_BYTE] = 2;
}

void prep_response(struct message_t *message, enum rq_t rq, void *payload, size_t payload_size)
{
    set_query_byte(message, rq);
    set_response_byte(message, 1);

    memcpy(message->payload, payload, payload_size);
}

bool are_remaining_bytes_valid(struct message_t message)
{
    /* check if all unused bits are set to 0 */
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (i == RESPONSE_BYTE || i == QUERY_BYTE)
            continue;

        if (message.code[i] != 0)
            return false;
    }

    return true;
}

bool is_query_valid(struct message_t message)
{
    if (get_response_byte(message) != 0 || !are_remaining_bytes_valid(message))
        return false;
    if (get_query_byte(message) != 1 && get_query_byte(message) != 2)
        return false;

    return true;
}

void determine_and_set_query_type(struct message_t *message)
{
    if (!is_query_valid(*message))
        return;

    enum rq_t set_to = -1;
    if (get_query_byte(*message) == 1)
        set_to = SQUARE;
    else if (get_query_byte(*message) == 2)
        set_to = DATE;

    message->rq = set_to;
}

double get_number_from_message(struct message_t message)
{
    double number = -1;
    memcpy(&number, message.payload, sizeof(double));

    return number;
}