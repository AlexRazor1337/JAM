#include "client.h"

gchar *init_user_image(gchar *login) {
    gchar *image = NULL;
    gchar *path = NULL;

    path = strjoin(path, "resource/images/users/");
    path = strjoin(path, login);
    path = strjoin(path, ".png");

    FILE *file = fopen(path, "r");

    if (file) {
        image = strdup(path);
        fclose(file);
    } else {
        image = strdup("resource/images/users/default.png");
    }

    strdel(&path);

    return image;
}
