#include "client.h"

void uchat_settings_close(GtkWidget *button, t_main_struct *main_struct) {
    gtk_widget_hide(main_struct->settings_box);
    gtk_widget_show_all(main_struct->uchat_box);

    uchat_show_current_chat(main_struct);
    uchat_switch_mainbar_size(NULL, main_struct);
    uchat_switch_mainbar_size(NULL, main_struct);

    gtk_widget_hide(gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(main_struct->sidebar_scrolled_chats)));
    gtk_widget_hide(gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(main_struct->mainbar_scrolled_chat)));
    gtk_widget_hide(gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(main_struct->stickerbar_scrolled)));

    if (!button) return;
}
