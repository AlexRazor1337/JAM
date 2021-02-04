#ifndef JAM_HEADER_H
#define JAM_HEADER_H

#include "../../libs/sqlite/sqlite3.h"
#include "../../libs/list/list.h"
#include "../../libs/dyad/dyad.h"

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
#include <netdb.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <time.h>
#include <sys/resource.h> //TODO REmove
#define SERVER_PORT 8000
extern int errno;


typedef struct s_connection {
    int id;
    int uid;
    void (*dataListener)(dyad_Event *);
    dyad_Stream *stream;
}              t_connection;


/* Signal handler to handle SIGTERM and SIGINT signals. */
void signal_handler(int signal_number);

int generate_unique_id(t_list *connections);
t_connection *find_node(int id, t_list *connections);
t_connection *find_node_uid(int uid, t_list *connections);
bool is_dir_exists(char* name);

int db_exec(sqlite3 *db, char* querry, char** result);

char *mx_itoa(int n);
int codepoint_len(const uint32_t cp); /* len of associated utf-8 char */
int utf8_len(const char ch);          /* len of utf-8 encoded char */

char *to_utf8(const uint32_t cp);
uint32_t to_cp(const char chr[4]);
#endif //JAM_HEADER_H
