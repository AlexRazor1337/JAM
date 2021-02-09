#ifndef JAM_HEADER_H
#define JAM_HEADER_H
#define _POSIX_C_SOURCE 200809L

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>  //TODO REmove
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../../libs/dyad/dyad.h"
#include "../../libs/list/list.h"
#include "../../libs/sqlite/sqlite3.h"

#define SERVER_PORT 8000
#define JSON_OP_SIZE 1024
extern int errno;

typedef struct s_connection {
    int id;
    int uid;
    void (*dataListener)(dyad_Event *);
    dyad_Stream *stream;
} t_connection;

/* Signal handler to handle SIGTERM and SIGINT signals. */
void signal_handler(int signal_number);

int generate_unique_id(t_list *connections);
t_connection *find_node(int id, t_list *connections);
t_connection *find_node_uid(int uid, t_list *connections);
bool is_dir_exists(char *name);

int db_exec(sqlite3 *db, char *querry, char **result);

char *mx_itoa(int n);
int codepoint_len(const uint32_t cp); /* len of associated utf-8 char */
int utf8_len(const char ch);          /* len of utf-8 encoded char */

char *to_utf8(const uint32_t cp);
uint32_t to_cp(const char chr[4]);
#endif  //JAM_HEADER_H