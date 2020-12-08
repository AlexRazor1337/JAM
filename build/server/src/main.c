#include "../inc/header.h"

void db_prepare(sqlite3 *db) {
    db_exec(db, "CREATE TABLE IF NOT EXISTS users (id integer PRIMARY KEY AUTOINCREMENT, name varchar, login varchar, password varchar, last_visit datetime);", NULL);
    db_exec(db, "CREATE TABLE IF NOT EXISTS messages (id_from integer, id_to integer, text text, timestamp datetime, attachment integer);", NULL);
    db_exec(db, "CREATE TABLE IF NOT EXISTS files (id integer PRIMARY KEY AUTOINCREMENT, hash varchar, name varchar, extension varchar);", NULL);
}

int max(int x, int y)
{
    if (x > y)
        return x;
    else
        return y;
}

typedef struct s_connection {
    int id;
    char address[256];
}              t_connection;


t_connection connections[10];
int tcp_fd, udp_fd;

int main() {
    
    sqlite3 *db;
    const unsigned char* result;

    printf("INITIALIZING JAM SERVER 🚑:\n");
    printf("INITIALIZING SQLITE: ");
    #pragma region db_init
    if (!is_dir_exists("server_data"))
        mkdir("server_data", 0755);

    int rc = sqlite3_open("server_data/main.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    db_prepare(db);
    #pragma endregion db_init

    db_exec(db, "SELECT SQLITE_VERSION()", &result);
    printf("VER. %s\n", result);
    
    int new_socket_fd;
    struct sockaddr_in address;
    pthread_attr_t pthread_attr;
    pthread_arg_t *pthread_arg;
    pthread_t pthread;
    socklen_t client_address_len;

    (void) pthread;
    #pragma region sockets_init
    /* Initialise IPv4 address. */
    memset(&address, 0, sizeof address);
    address.sin_family = AF_INET;
    address.sin_port = htons(SERVER_PORT);
    address.sin_addr.s_addr = INADDR_ANY;


    
    /* Create TCP socket. */
    if ((tcp_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Error creating TCP socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if ((udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) { 
        fprintf(stderr, "Error creating socket: UDP %s\n", strerror(errno));
        exit(EXIT_FAILURE); 
    } 

    /* TCP Bind address to socket. */
    if (bind(tcp_fd, (struct sockaddr *)&address, sizeof address) == -1 || bind(udp_fd, (struct sockaddr *)&address, sizeof address)) {
        fprintf(stderr, "Error binding sockets: %s\n", strerror(errno));
        close(tcp_fd);
        close(udp_fd);
        exit(EXIT_FAILURE);
    }

    /* Listen on socket. */
    if (listen(tcp_fd, INT32_MAX) == -1) {
        fprintf(stderr, "Error listening to socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Assign signal handlers to signals. */
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGTERM, signal_handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    /* Initialise pthread attribute to create detached threads. */
    if (pthread_attr_init(&pthread_attr) != 0) {
        perror("pthread_attr_init");
        exit(1);
    }
    if (pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED) != 0) {
        perror("pthread_attr_setdetachstate");
        exit(1);
    }

    #pragma endregion sockets_init

    int connections_count = 0;
    fd_set rset;
    int max_fdp = max(tcp_fd, udp_fd) + 1;
    while (true) {
        FD_SET(tcp_fd, &rset);
        FD_SET(udp_fd, &rset);

        select(max_fdp, &rset, NULL, NULL, NULL);

        pthread_arg = (pthread_arg_t *) malloc(sizeof *pthread_arg);
        if (!pthread_arg) {
            perror("malloc");
            continue;
        }
        if (FD_ISSET(tcp_fd, &rset)) {
            /* Accept connection to client. */
            client_address_len = sizeof pthread_arg->client_address;
            new_socket_fd = accept(tcp_fd, (struct sockaddr *)&pthread_arg->client_address, &client_address_len);
            if (new_socket_fd == -1) {
                perror("accept");
                free(pthread_arg);
                continue;
            }
            printf("Accepted connection\n");
            /* Initialise pthread argument. */
            pthread_arg->new_socket_fd = new_socket_fd;
            pthread_arg->id = 0;// ++connections_count;
            /* TODO: Initialise arguments passed to threads here. See lines 22 and
            * 139.
            */

            /* Create thread to serve connection to client. */
            if (pthread_create(&pthread, &pthread_attr, pthread_routine, (void *)pthread_arg) != 0) {
                perror("pthread_create");
                free(pthread_arg);
                continue;
            }
        } else if(FD_ISSET(udp_fd, &rset)) {
        

            char buf[512];
            if(recvfrom(udp_fd, buf, 512, 0, (struct sockaddr *)&pthread_arg->client_address, &client_address_len) == -1)
                perror("recvfrom()");

            printf("Recieved packet from %s: %d\nData: %s\n", inet_ntoa(pthread_arg->client_address.sin_addr), ntohs(address.sin_port), buf);
            if (buf[0] == '1') {
                printf("Client with id %s sent hello, registering\n", &buf[1]);
                connections[connections_count].id = atoi(&buf[1]);
                strcpy(connections[connections_count].address, inet_ntoa(pthread_arg->client_address.sin_addr));
                ++connections_count;
            }

        }
    }



    /* close(tcp_fd);
     * TODO: If you really want to close the socket, you would do it in
     * signal_handler(), meaning tcp_fd would need to be a global variable.
     */

    close(tcp_fd);
    close(udp_fd);
    sqlite3_close(db);
    return 0;
}


void *pthread_routine(void *arg) {
    pthread_arg_t *pthread_arg = (pthread_arg_t *)arg;
    int new_socket_fd = pthread_arg->new_socket_fd;
    struct sockaddr_in client_address = pthread_arg->client_address;
    /* TODO: Get arguments passed to threads here. See lines 22 and 116. */
    (void) client_address;
    free(arg);
    write(new_socket_fd, "0", 2);
    printf("here");
    char buf[256];
    int rc = 0;
    if ((rc = read(new_socket_fd, &buf, 256)) > 0) {
        printf("Got from client: %s", buf);
    }


    for (int i = 0; i < 10; i++) {
        if (connections[i].id == atoi(&buf[0])) {
            printf("%s buf TO %s \n", buf, connections[i].address);
            struct hostent *server_host = gethostbyname(connections[i].address);
            struct sockaddr_in server_address;
            /* Initialise IPv4 server address with server host. */
            memset(&server_address, 0, sizeof server_address);
            server_address.sin_family = AF_INET;
            server_address.sin_port = htons(8000);

            memcpy(&server_address.sin_addr.s_addr, server_host->h_addr, server_host->h_length);

            int rc = sendto(udp_fd, buf, strlen(buf), 0, (const struct sockaddr*) &server_address, sizeof(server_address));
            printf("rc %d", rc);
            if (rc < 0) {
                perror("rc ");
            }
        }
    }
    /* TODO: Put client interaction code here. For example, use
     * write(new_socket_fd,,) and read(new_socket_fd,,) to send and receive
     * messages with the client.
     */

    close(new_socket_fd);
    return NULL;
}

void signal_handler(int signal_number) {
    (void) signal_number;
    /* TODO: Put exit cleanup code here. */
    exit(0);
}
