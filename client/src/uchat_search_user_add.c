#include "client.h"

void uchat_search_user_add(GtkWidget *button, t_main_struct *main_struct) {
    gchar *user_login = (gchar *)gtk_entry_get_text(GTK_ENTRY(main_struct->search_box_search_entry));

    if (strcmp(user_login, "")) {
        addUser(user_login);

        while (!client.json_data) {
            continue;
        }

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

        gtk_entry_set_text(GTK_ENTRY(main_struct->search_box_search_entry), "");
    }

    if (!button) return;
}
