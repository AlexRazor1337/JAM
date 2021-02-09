#include "client.h"

void strdel(gchar **str) {
    if (!str || !(*str)) {
        return;
    }

    free((*str));
    (*str) = NULL;
}
