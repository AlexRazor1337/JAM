#include "client.h"

void uchat_settings_set_password(GtkWidget *entry) {
    gchar *password = (gchar *)gtk_entry_get_text(GTK_ENTRY(entry));

    if (strlen(password) < 4) {
        gtk_widget_set_name(entry, "profile_password_entry_error");
    } else {
        gtk_widget_set_name(entry, "profile_password_entry");
    }
}
