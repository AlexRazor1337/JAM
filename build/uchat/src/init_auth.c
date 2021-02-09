#include "client.h"

t_auth *init_auth() {
    t_auth *auth = (t_auth *)malloc(sizeof(t_auth));

    auth->login = strdup("");
    auth->username = strdup("");
    auth->password = strdup("");
    auth->password_repeat = strdup("");
    auth->image = init_user_image(auth->login);

    return auth;
}
