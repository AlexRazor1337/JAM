#if defined(__linux__)
#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#define h_addr h_addr_list[0] /* for backward compatibility */
#endif

#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../libs/dyad/dyad.h"
#include "../../libs/list/list.h"
#include "../../libs/aes/aes.h"

#define SERVER_NAME_LEN_MAX 255
#define SERVER_PORT 8000
#define SERVER_ADRESS "127.0.0.1"  //"10.11.7.9"

enum State { UNAUTH,
             AUTH,
             AUTH_FAILED };

typedef struct s_client {
    char *login;
    char *username;
    char *password;
    char *json_data;
    unsigned long int uid;
    enum State state;
}              t_client;

typedef struct s_connect_data {
    pthread_t thread;
    pthread_attr_t attr;
    bool to_sign_up;
}              t_connect_data;

extern t_client client;
extern dyad_Stream *server_stream;

void *serverInit(void *argument);
void addUser(char *login);
void connectToServer();
void sendMessage(size_t id, char *message, int type);
void sendFileMessage(size_t id, char *filename, FILE *file);

int codepoint_len(const uint32_t cp); /* len of associated utf-8 char */
int utf8_len(const char ch);          /* len of utf-8 encoded char */

char *to_utf8(const uint32_t cp);
uint32_t to_cp(const char chr[4]);
