#include "client.h"

void uchat_seach_user_add_network() {
    guint original_size = user_list_size(main_struct->user_list);
    user_list_parse_from_json(&main_struct->user_list, client.json_data);
    guint new_size = user_list_size(main_struct->user_list);

    strdel(&client.json_data);

    if (original_size < new_size) {
        t_user_list *user_list_cpy = main_struct->user_list;

        while (user_list_cpy->next) {
            user_list_cpy = user_list_cpy->next;
        }

        uchat_sidebar_chat_new(main_struct, user_list_cpy->user);
        uchat_mainbar_chat_new(main_struct, user_list_cpy->user);

        gtk_widget_show_all(main_struct->sidebar_scrolled_chats);
    }
}
