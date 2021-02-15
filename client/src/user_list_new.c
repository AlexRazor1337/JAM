#include "client.h"

t_user_list *user_list_new(guint id, gchar *login, gchar *username) {
    t_user_list *user_list = (t_user_list *)malloc(sizeof(t_user_list));

    user_list->user = user_new(id, login, username);
    user_list->next = NULL;

    return user_list;
}
