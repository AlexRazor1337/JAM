#include "client.h"

void sign_up_update_password_repeat(GtkWidget *entry, t_main_struct *main_struct) {
    gtk_widget_hide(main_struct->password_is_empty);
    gtk_widget_hide(main_struct->password_repeat_is_not_identical);
    gtk_widget_hide(main_struct->auth_is_failed);

    gchar *password_repeat = (gchar *)gtk_entry_get_text(GTK_ENTRY(entry));

    if (!strcmp(main_struct->auth->password, "")) {
        gtk_widget_show_all(main_struct->password_is_empty);
    } else if (!strcmp(password_repeat, "") || strncmp(password_repeat, main_struct->auth->password, strlen(password_repeat))) {
        gtk_widget_show_all(main_struct->password_repeat_is_not_identical);
    }

    strdel(&main_struct->auth->password_repeat);
    main_struct->auth->password_repeat = strdup(password_repeat);
}
