#include "client.h"

void user_list_print(t_user_list *user_list) {
    while (user_list) {
        g_print("{\n\t\"id\": %d\n\t\"login\": %s\n\t\"username\": %s\n\t\"image\": %s\n}\n", user_list->user->id, user_list->user->login, user_list->user->username, user_list->user->image);

        user_list = user_list->next;
    }
}
