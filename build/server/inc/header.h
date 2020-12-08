#ifndef JAM_HEADER_H
#define JAM_HEADER_H

#include "../../libs/sqlite/sqlite3.h"

#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include <arpa/inet.h>


#define SERVER_PORT 8000
extern int errno;
typedef struct pthread_arg_t {
    int new_socket_fd;
    struct sockaddr_in client_address;
    /* TODO: Put arguments passed to threads here. See lines 116 and 139. */
    int id;
} pthread_arg_t;

/* Thread routine to serve connection to client. */
void *pthread_routine(void *arg);

/* Signal handler to handle SIGTERM and SIGINT signals. */
void signal_handler(int signal_number);


bool is_dir_exists(char* name);

int db_exec(sqlite3 *db, char* querry, const unsigned char** result);


int codepoint_len(const uint32_t cp); /* len of associated utf-8 char */
int utf8_len(const char ch);          /* len of utf-8 encoded char */
 
char *to_utf8(const uint32_t cp);
uint32_t to_cp(const char chr[4]);
#endif //JAM_HEADER_H
