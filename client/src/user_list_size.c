#include "client.h"

guint user_list_size(t_user_list *user_list) {
    guint size = 0;

    while (user_list) {
        user_list = user_list->next;
        size++;
    }

    return size;
}
