#include "client.h"

gchar *strjoin(const gchar *s1, const gchar *s2) {
    if (!s1 && !s2) {
        return NULL;
    } else if (!s1) {
        return strdup(s2);
    } else if (!s2) {
        return strdup(s1);
    }

    gchar *new = (gchar *)malloc(strlen(s1) + strlen(s2) + 1);

    strcpy(new, s1);
    strcat(new, s2);

    return new;
}
