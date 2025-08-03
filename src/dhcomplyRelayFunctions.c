#include "dhcomplyRelayFunctions.h"

void *create_buffer() {
    buffer *packet_buffer = (buffer *)malloc(sizeof(buffer));

	packet_buffer->in_index = 0;
    packet_buffer->out_index = 0;
    packet_buffer->count = 0;
    pthread_mutex_init(&packet_buffer->mutex, NULL);
	pthread_cond_init(&packet_buffer->cond_not_full, NULL);
    pthread_cond_init(&packet_buffer->cond_not_empty, NULL);

    for (int message = 0; message < MESSAGE_COUNT; message++) {
        memset(packet_buffer->packets[message], 0, MESSAGE_BYTES);
    }

    return packet_buffer;
}

void *parse_relay_packet(void *packet_buffer, uint8_t *packet) {
    int sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    valid_socket(sockfd);

    pthread_mutex_lock(&packet_buffer->mutex);

    while (buffer->count == message_count) {
		pthread_cond_wait(&packet_buffer->cond_not_full, &packet_buffer->mutex);
	}

    // Add packet to the buffer
    memcpy(buffer->packets[rear % MESSAGE_COUNT], packet, MAX_PACKET_SIZE);

	buffer->count++;
    buffer->rear++;

    pthread_cond_signal(&packet_buffer->cond_not_empty);

    pthread_cond_signal(&packet_buffer->cond_not_empty);
	pthread_mutex_unlock(&packet_buffer->mutex);

    close(sockfd);
    return NULL;
}

void *send_relay_packet(void *packet_buffer) {
	int sockfd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    valid_socket(sockfd);

    pthread_mutex_lock(&packet_buffer->mutex);

    while (!buffer->count) {
		pthread_cond_wait(&packet_buffer->cond_not_full, &packet_buffer->mutex);
	}

   struct sockaddr_in6 dest_addr;
   memset(&dest_addr, 0, sizeof(dest_addr));
   dest_addr.sin6_family = AF_INET6;
   dest_addr.sin6_port = htons(DEST_PORT);
   inet_pton(AF_INET6, ALL_DHCP_RELAY_AGENTS_AND_SERVERS, &dest_addr.sin6_addr);
   sendto(sockfd, buffer->packets[buffer->in_index % MESSAGE_COUNT], PACKET_SIZE, 0,(struct sockaddr *)&dest_addr, sizeof(dest_addr));

    buffer->count--;
	buffer->in_index++;

    // Signal that the buffer is not full
    pthread_cond_signal(&packet_buffer->cond_not_full);

    // Signal that there is at least one packet in the buffer

   pthread_signal(&packet_buffer->cond_not_empty);
   pthread_unlock(&packet_buffer->mutex);

}

int check_for_message(int sockfd, uint8_t *packet, int type) {
    fd_set read_fds;
    struct timeval timeout;
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    int ready = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);
    if (ready > 0 && FD_ISSET(sockfd, &read_fds)) {
        uint8_t buffer[MAX_PACKET_SIZE];
        ssize_t len = recv(sockfd, buffer, sizeof(buffer), 0);
        memcpy(packet, buffer, len);
        if (buffer[0] == type) {
            return len;
        }
    }

    return 0;
}