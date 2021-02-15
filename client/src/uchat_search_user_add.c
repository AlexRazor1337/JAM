#include "client.h"

void uchat_search_user_add(GtkWidget *button, t_main_struct *main_struct) {
    gchar *user_login = (gchar *)gtk_entry_get_text(GTK_ENTRY(main_struct->search_box_search_entry));

    if (strcmp(user_login, "") && strcmp(user_login, main_struct->auth->login)) {
        addUser(user_login);
        gtk_entry_set_text(GTK_ENTRY(main_struct->search_box_search_entry), "");
        gtk_widget_hide(main_struct->search_box);
    }

    if (!button) return;
}
