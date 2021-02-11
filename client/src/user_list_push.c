#include "client.h"

void user_list_push(t_user_list **user_list, guint id, gchar *login, gchar *username) {
    t_user_list *user_list_cpy = *user_list;

    while (user_list_cpy->next)
        user_list_cpy = user_list_cpy->next;

    user_list_cpy->next = user_list_new(id, login, username);
}
