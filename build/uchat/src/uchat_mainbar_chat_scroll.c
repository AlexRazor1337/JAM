#include "client.h"

void uchat_mainbar_chat_scroll(t_main_struct *main_struct) {
    while (gtk_events_pending()) {
        gtk_main_iteration();

        GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(main_struct->mainbar_scrolled_chat));
        gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj));
        gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(main_struct->mainbar_scrolled_chat), adj);
    }
}
