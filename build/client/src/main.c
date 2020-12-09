#include "../inc/header.h"

int tcp_connect(struct sockaddr_in *server_address) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
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


// void ReadXBytes(int socket, unsigned int x, void* buffer)
// {
//     int bytesRead = 0;
//     int result;
//     while (bytesRead < x)
//     {
//         result = read(socket, buffer + bytesRead, x - bytesRead);
//         if (result < 1 )
//         {
//             // Throw your error.
//         }

//         bytesRead += result;
//     }
// }

unsigned char *create_packet(char* message) {
    unsigned int len = strlen(message);


    unsigned char *outBuf = malloc(sizeof(len)+sizeof(message));
    unsigned char *p = outBuf;

    // Serialize "x" into outBuf
    uint32_t neX = htonl(len);
    memcpy(p, &neX, sizeof(neX));
    p += sizeof(neX);

    // Serialize "y" into outBuf
    memcpy(p, message, strlen(message));
    p += strlen(message);


    printf("data: %s\n", outBuf);
    return outBuf;
}

int main() {
    char server_name[SERVER_NAME_LEN_MAX + 1] = "10.11.7.9";
    int server_port, tcp_fd;
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

    server_host = gethostbyname("0.0.0.0");
    struct sockaddr_in bind_addr;
    memset(&bind_addr, 0, sizeof bind_addr);
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = htons(server_port);
    memcpy(&bind_addr.sin_addr.s_addr, server_host->h_addr, server_host->h_length);

    tcp_fd = tcp_connect(&server_address);
    fcntl(tcp_fd, F_SETFL, O_NONBLOCK);
    char buf[256];
    char *line = NULL;
    size_t len = 0;
    printf("Input your id: ");
    getline(&line, &len, stdin);
    strcat(buf, "1");
    strcat(buf, line);
    write(tcp_fd, buf, strlen(buf));
    
    while (true) { //TODO Implement heartbeat
        char buf2[256];
        printf("Input action: 1 - read, 2 - write: ");
        getline(&line, &len, stdin);
        if (line[0] == '1') {
            while (read(tcp_fd, &buf, 256) > 0) {
                char *cleared = buf + 2;
                printf("Message: %s\n", cleared);
            }
        } else {
            printf("Input getter id and text: ");
            getline(&line, &len, stdin);
            strcat(buf2, "2");
            strcat(buf2, line);
            write(tcp_fd, buf2, sizeof(line));
        }
    }


    /* TODO: Put server interaction code here. For example, use
     * write(socket_fd,,) and read(socket_fd,,) to send and receive messages
     * with the client.
     */

    close(tcp_fd);
    return 0;
}
