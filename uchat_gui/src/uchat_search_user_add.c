#include "client.h"

void uchat_search_user_add(GtkWidget *button, t_main_struct *main_struct) {
    gchar *add = (gchar *)gtk_entry_get_text(GTK_ENTRY(main_struct->search_box_search_entry));

    if (strcmp(add, "")) {
        g_print("Add request: %s\n", add);
        gtk_entry_set_text(GTK_ENTRY(main_struct->search_box_search_entry), "");
    }

    if (!button) return;
}
