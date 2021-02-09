#include "client.h"

t_user *user_new(gchar *login, gchar *username) {
    t_user *user = (t_user *) malloc(sizeof(t_user));

    user->login = strdup(login);
    user->username = strdup(username);
    user->image = strdup("resource/images/users/default.png");

    return user;
}
