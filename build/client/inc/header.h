#if defined(__linux__)
#define  _GNU_SOURCE
#define  _POSIX_C_SOURCE 200809L
#define h_addr h_addr_list[0] /* for backward compatibility */
#endif

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#define SERVER_NAME_LEN_MAX 255
#include "../../libs/dyad/dyad.h"


typedef struct s_client {
    unsigned long int uid;
    unsigned short state;
} t_client;


int codepoint_len(const uint32_t cp); /* len of associated utf-8 char */
int utf8_len(const char ch);          /* len of utf-8 encoded char */

char *to_utf8(const uint32_t cp);
uint32_t to_cp(const char chr[4]);
