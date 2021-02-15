#include "client.h"

void uchat_settings_set_username(GtkWidget *entry) {
    gchar *username = (gchar *)gtk_entry_get_text(GTK_ENTRY(entry));

    if (strlen(username) < 4) {
        gtk_widget_set_name(entry, "profile_username_entry_error");
    } else {
        gtk_widget_set_name(entry, "profile_username_entry");
    }
}
