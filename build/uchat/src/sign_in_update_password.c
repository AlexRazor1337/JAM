#include "client.h"

void sign_in_update_password(GtkWidget *entry, t_main_struct *main_struct) {
    gtk_widget_hide(main_struct->password_is_empty);
    gtk_widget_hide(main_struct->password_is_too_short);

    gchar *password = (gchar *)gtk_entry_get_text(GTK_ENTRY(entry));

    if (!strcmp(password, "")) {
        gtk_widget_show_all(main_struct->password_is_empty);
    } else if (strlen(password) < 6) {
        gtk_widget_show_all(main_struct->password_is_too_short);
    }

    strdel(&main_struct->auth->password);
    main_struct->auth->password = strdup(password);
}
