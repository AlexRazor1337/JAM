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
#include "../../libs/vec/vec.h"
#include "../../libs/aes/aes.h"

#define SERVER_PORT 8000
#define LOGIN_SIZE 16
#define PASSWORD_SIZE 32
#define USERNAME_SIZE 32
extern int errno;

typedef struct s_connection {
    int id;
    int uid;
    dyad_Stream *stream;
} t_connection;

void signal_handler(int signal_number);

int generate_unique_id(t_list *connections);
t_connection *find_node(int id, t_list *connections);
t_connection *find_node_uid(int uid, t_list *connections);
bool is_dir_exists(char *name);
char *jsonlist_from_jsones(vec_str_t v, int bsize);
t_connection *create_connection(dyad_Stream *stream, t_list *connections);
void strdel(char **str);

int db_exec(sqlite3 *db, char *querry, char **result);

char *mx_itoa(int n);
#endif  //JAM_HEADER_H
