#include <string.h>

#include "server_parse_query.h"

#define GET_BIT(x, pos) (x & (1UL << pos))
#define SET_BIT(x, pos) (x |= (1U << pos))

int get_query_bit(struct message_t message)
{
    return GET_BIT(message.code, QUERY_BIT);
}

int get_response_bit(struct message_t message)
{
    return GET_BIT(message.code, RESPONSE_BIT);
}

void set_response_bit(struct message_t *message)
{
    SET_BIT(message->code, RESPONSE_BIT);
}

void set_query_bit(struct message_t *message, enum rq_t rq)
{
    if (rq != SQUARE && rq != DATE)
        return;

    SET_BIT(message->code, QUERY_BIT);
    message->code |= rq;
}

void prep_response(struct message_t *message, enum rq_t rq, void *payload, size_t payload_size)
{
    set_query_bit(message, rq);
    set_response_bit(message);

    memcpy(message->payload, payload, payload_size);
}

bool are_remaining_bits_valid(struct message_t message)
{
    /* check if all unused bits are set to 0 */
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (i == RESPONSE_BIT || i == QUERY_BIT)
            continue;

        if (GET_BIT(message.code, i) != 0)
            return false;
    }

    return true;
}

bool is_query_valid(struct message_t message)
{
    if (get_response_bit(message) != 0 || !are_remaining_bits_valid(message))
        return false;
    if (get_query_bit(message) != 1 && get_query_bit(message) != 2)
        return false;

    return true;
}

void determine_and_set_query_type(struct message_t *message)
{
    enum rq_t set_to = -1;
    if (get_query_bit(*message) == 1)
        set_to = SQUARE;
    else if (get_query_bit(*message) == 2)
        set_to = DATE;

    message->rq = set_to;
}

double get_number_from_message(struct message_t message)
{
    double number = -1;
    memcpy(&number, message.payload, sizeof(double));

    return number;
}