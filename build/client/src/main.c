#include "../inc/header.h"

int tcp_connect(struct sockaddr_in *server_address) {
    int socket_fd;
    /* Create TCP socket. */
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* Connect to socket with server address. */
    if (connect(socket_fd, (struct sockaddr *) &(*server_address), sizeof(*server_address)) == -1) {
		perror("connect");
        exit(1);
	}

    return socket_fd;
}

int udp_connect(struct sockaddr_in *server_address) {
    int udp_fd;
    if ((udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket creation failed");
        exit(EXIT_FAILURE);
    }
    (void) server_address;
    return udp_fd;
}

int main() {
    char server_name[SERVER_NAME_LEN_MAX + 1] = "127.0.0.1";;
    int server_port, tcp_fd, udp_fd;
    struct hostent *server_host;
    struct sockaddr_in server_address;


    /* Get server port from command line arguments or stdin. */
    server_port = 8000;

    /* Get server host from server name. */
    server_host = gethostbyname(server_name);

    /* Initialise IPv4 server address with server host. */
    memset(&server_address, 0, sizeof server_address);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    memcpy(&server_address.sin_addr.s_addr, server_host->h_addr, server_host->h_length);
    tcp_fd = tcp_connect(&server_address);
    udp_fd = udp_connect(&server_address);

    
    //char buf[256];
    char *line = NULL;
    size_t len = 0;
    ssize_t rd = 0;

    printf("Input message: ");
    rd = getline(&line, &len, stdin);
    // write(tcp_fd, line, strlen(line));
    sendto(udp_fd, line, strlen(line), 0, (const struct sockaddr*)&server_address, sizeof(server_address));
    // if (read(socket_fd, &buf, 256) > 0) {
    //     printf("Got from server: %s", buf);
    // }

    // switch (buf[0]) {
    //     case '0':
    //         printf("Input message: ");
    //         rd = getline(&line, &len, stdin);
    //         write(socket_fd, line, strlen(line));
    //         break;
    //     default:
    //         break;
    // }
    // while (read(socket_fd, &buf, 256) > 0) {
    //     printf("%s", buf);
    // }

    /* TODO: Put server interaction code here. For example, use
     * write(socket_fd,,) and read(socket_fd,,) to send and receive messages
     * with the client.
     */

    close(tcp_fd);
    close(udp_fd);
    return 0;
}
