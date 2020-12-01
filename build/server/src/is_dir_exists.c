#include "../inc/header.h"

bool is_dir_exists(char* name) {
    struct stat st = {0};

    if (stat(name, &st) == -1) { // If directory doesn't exists
        return false;
    }
    return true;
}
