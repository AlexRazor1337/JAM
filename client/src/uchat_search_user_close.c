#include "client.h"

void uchat_search_user_close(GtkWidget *button, t_main_struct *main_struct) {
    gtk_widget_hide(main_struct->search_box);
    gtk_entry_set_text(GTK_ENTRY(main_struct->search_box_search_entry), "");

    if (!button) return;
}
