#include "client.h"

void sign_in_update_login(GtkWidget *entry, t_main_struct *main_struct) {
    gtk_widget_hide(main_struct->login_is_empty);
    gtk_widget_hide(main_struct->login_is_too_short);
    gtk_widget_hide(main_struct->login_is_not_unique);

    gchar *login = (gchar *)gtk_entry_get_text(GTK_ENTRY(entry));

    if (!strcmp(login, "")) {
        gtk_widget_show_all(main_struct->login_is_empty);
    } else if (strlen(login) < 4) {
        gtk_widget_show_all(main_struct->login_is_too_short);
    }

    strdel(&main_struct->auth->login);
    main_struct->auth->login = strdup(login);
}