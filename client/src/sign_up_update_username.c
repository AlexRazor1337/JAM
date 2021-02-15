#include "client.h"

void sign_up_update_username(GtkWidget *entry, t_main_struct *main_struct) {
    gtk_widget_hide(main_struct->username_is_empty);
    gtk_widget_hide(main_struct->username_is_too_short);
    gtk_widget_hide(main_struct->auth_is_failed);

    gchar *username = (gchar *)gtk_entry_get_text(GTK_ENTRY(entry));

    if (!strcmp(username, "")) {
        gtk_widget_show_all(main_struct->username_is_empty);
    } else if (strlen(username) < 4) {
        gtk_widget_show_all(main_struct->username_is_too_short);
    }

    strdel(&main_struct->auth->username);
    main_struct->auth->username = strdup(username);
}
