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
int tcp_fd;
t_list *connections_list = NULL;

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
    int option = 1;
    setsockopt(tcp_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    /* TCP Bind address to socket. */
    if (bind(tcp_fd, (struct sockaddr *)&address, sizeof address) == -1) {
        fprintf(stderr, "Error binding sockets: %s\n", strerror(errno));
        close(tcp_fd);
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

    while (true) {
        pthread_arg = (pthread_arg_t *) malloc(sizeof *pthread_arg);
        if (!pthread_arg) {
            perror("malloc");
            continue;
        }
        /* Accept connection to client. */
        client_address_len = sizeof pthread_arg->client_address;
        new_socket_fd = accept(tcp_fd, (struct sockaddr *)&pthread_arg->client_address, &client_address_len);
        if (new_socket_fd == -1) {
            //perror("accept");
            free(pthread_arg);
            continue;
        }
        printf("Accepted connection\n");
        t_list *carret = connections_list;
        while (carret) {
            if (carret->data) {
                connection_t *con = carret->data;
                printf("FD: %d ID: %d", con->fd, con->id);

            }
            
            carret = carret->next;
        }
        //fcntl(tcp_fd, F_SETFL, O_NONBLOCK); /* Change the socket into non-blocking state	*/
        fcntl(new_socket_fd, F_SETFL, O_NONBLOCK);
        /* Initialise pthread argument. */
        pthread_arg->new_socket_fd = new_socket_fd;
        pthread_arg->id = ++connections_count;
        
        connection_t *new_connection = malloc(sizeof(connection_t));
        new_connection->fd = new_socket_fd;
        mx_push_back(&connections_list, new_connection);
        /* TODO: Initialise arguments passed to threads here. See lines 22 and
        * 139.
        */

        /* Create thread to serve connection to client. */
        if (pthread_create(&pthread, &pthread_attr, pthread_routine, (void *)pthread_arg) != 0) {
            perror("pthread_create");
            free(pthread_arg);
            continue;
        }
    }
    /* close(tcp_fd);
     * TODO: If you really want to close the socket, you would do it in
     * signal_handler(), meaning tcp_fd would need to be a global variable.
     */

    close(tcp_fd);
    sqlite3_close(db);
    return 0;
}


void *pthread_routine(void *arg) {
    pthread_arg_t *pthread_arg = (pthread_arg_t *)arg;
    int new_socket_fd = pthread_arg->new_socket_fd;

    free(arg);
    
    printf("here %d \n", new_socket_fd);
    
    char buf[256];
    while(true) {
        
        memset(buf, 0, 256);
		int n = recv(new_socket_fd, buf, sizeof(buf), 0);
        if (n < 1){ 
            continue;
            // perror("recv - non blocking \n");
            //     printf("Round %d, and the data read size is: n=%d \n", new_socket_fd,n);
        }
        else{  
            buf[n] = '\0';
            printf("The string is: %s \n",buf);
            if (buf[0] == '1') {
                t_list *carret = connections_list;
                while (carret) {
                    connection_t *con = carret->data;
                    if (carret->data && con->fd == new_socket_fd) {
                        buf[0] = '0';
                        con->id = atoi(buf);
                        break;
                    }
                    carret = carret->next;
                }
            } else if (buf[0] == '2') { 
                t_list *carret = connections_list;
                while (carret) {
                    connection_t *con = carret->data;
                    printf("%d == %d \n", con->id, buf[1] - '0');
                    if (carret->data && con->id == buf[1] - '0') {
                        int rc = write(con->fd, buf, sizeof(buf));
                        printf("rc %d\n", rc);
                        break;
                    }
                    carret = carret->next;
                }
            }
            // if (send(new_fd, "Hello, world!\n", 14, 0) == -1)
            //     perror("send");
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
