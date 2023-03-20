#include <stdio.h>
#include "server_client_handler.h"


int main()
{
    initialize_server();

    while(1) {
        /* run server_listen on a thread */
        pthread_t server_listen_thread;
        printf("Starting server...\n");
        pthread_create(&server_listen_thread, NULL, server_listen, NULL);

        char c;
        if (scanf("%c", &c) == 'q') {
            printf("Quitting...\n");
            pthread_cancel(server_listen_thread);
            destroy_server();
            break;
        }
    }

    return 0;
}


