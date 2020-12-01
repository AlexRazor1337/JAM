#ifndef JAM_HEADER_H
#define JAM_HEADER_H

#include "../../libs/sqlite/sqlite3.h"
#include "stdlib.h"
#include "stdio.h"
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

bool is_dir_exists(char* name);

int db_exec(sqlite3 *db, char* querry, const unsigned char** result);

#endif //JAM_HEADER_H
