#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
#include "server_client_handler.h"

int main()
{
    /* run server_listen on a thread */
    pthread_t server_listen_thread;
    printf("Starting server...\n");
    initialize_server();
    pthread_create(&server_listen_thread, NULL, server_listen, NULL);

    while(1) {
        char c;
        scanf("%c%*c", &c);
        if (c == 'q') {
            printf("Quitting...\n");
            atomic_store(&terminate_server, 1);
            destroy_server();
            break;
        }
    }

    return 0;
}


