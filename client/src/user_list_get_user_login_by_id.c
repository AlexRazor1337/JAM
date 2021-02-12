#include "client.h"

gchar *user_list_get_user_login_by_id(t_user_list *user_list, guint id) {
    while (user_list) {
        if (user_list->user->id == id) {
            return user_list->user->login;
        }

        user_list = user_list->next;
    }

    return "";
}
