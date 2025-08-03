#include "dhcomplyRelayFunctions.h"

int main() {
    int sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    valid_socket(sockfd);
    uint8_t buffer[MESSAGE_COUNT][MESSAGE_BYTES];
    int in_index = 0;
    int out_index = 0;
    int count = 0;
    while (1) {
        pthread_t client_thread, server_thread;

        pthread_create(&client_thread, NULL, client_producer, NULL);
        pthread_create(&server_thread, NULL, server_consumer, NULL);

        pthread_join(client_thread, NULL);
        pthread_join(server_thread, NULL);

        // Reset state for the next round
        in_index = 0;
        out_index = 0;
        count = 0;
        memset(buffer, 0, sizeof(buffer));

        // Optional: Sleep before restarting
        printf("Restarting cycle...\n");
        sleep(1); // Wait 1 second before next round
    }

    // We never reach here, so you might skip destroying mutexes.
    return 0;
}
