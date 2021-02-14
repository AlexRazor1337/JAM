#include "client.h"

void *uchat_mainbar_chat_scroll_thread(void *args) {
    usleep(5000);
    (void) args;

    GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(main_struct->mainbar_scrolled_chat));
    gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj) - gtk_adjustment_get_page_size(adj));
    gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(main_struct->mainbar_scrolled_chat), adj);

    pthread_exit(NULL);
}
