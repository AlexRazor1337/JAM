#include "client.h"

void uchat_sidebar_chat_new(t_main_struct *main_struct, t_user *current) {
    GtkWidget *sidebar_currnet_chat_button;
    GtkWidget *sidebar_currnet_chat_fixed;
    GtkWidget *sidebar_currnet_chat_image;
    GtkWidget *sidebar_currnet_chat_username_label;
    GtkWidget *sidebar_currnet_chat_login_label;
    GtkWidget *sidebar_currnet_chat_status;

    sidebar_currnet_chat_button = gtk_button_new();
    gtk_widget_set_name(sidebar_currnet_chat_button, "sidebar_currnet_chat_button");
    gtk_widget_set_size_request(sidebar_currnet_chat_button, 270, 65);

    sidebar_currnet_chat_fixed = gtk_fixed_new();
    gtk_widget_set_name(sidebar_currnet_chat_fixed, "sidebar_currnet_chat_fixed");
    gtk_widget_set_size_request(sidebar_currnet_chat_fixed, 270, 65);

    sidebar_currnet_chat_image = gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_file_at_scale(current->image, 50, 50, FALSE, NULL));  // scale
    gtk_widget_set_name(sidebar_currnet_chat_image, "sidebar_currnet_chat_image");
    gtk_widget_set_size_request(sidebar_currnet_chat_image, 65, 65);

    sidebar_currnet_chat_username_label = gtk_label_new(current->username);
    gtk_widget_set_name(sidebar_currnet_chat_username_label, "sidebar_currnet_chat_username_label");

    sidebar_currnet_chat_login_label = gtk_label_new(current->login);
    gtk_widget_set_name(sidebar_currnet_chat_login_label, "sidebar_currnet_chat_login_label");

    sidebar_currnet_chat_status = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(sidebar_currnet_chat_status, "offline");
    gtk_widget_set_size_request(sidebar_currnet_chat_status, 10, 10);

    gtk_fixed_put(GTK_FIXED(sidebar_currnet_chat_fixed), sidebar_currnet_chat_image, 0, 0);
    gtk_fixed_put(GTK_FIXED(sidebar_currnet_chat_fixed), sidebar_currnet_chat_username_label, 65, 10);
    gtk_fixed_put(GTK_FIXED(sidebar_currnet_chat_fixed), sidebar_currnet_chat_login_label, 65, 35);
    gtk_fixed_put(GTK_FIXED(sidebar_currnet_chat_fixed), sidebar_currnet_chat_status, 50, 50);

    gtk_container_add(GTK_CONTAINER(sidebar_currnet_chat_button), sidebar_currnet_chat_fixed);
    gtk_box_pack_start(GTK_BOX(main_struct->sidebar_scrolled_chats_box), sidebar_currnet_chat_button, FALSE, FALSE, 0);

    g_signal_connect(sidebar_currnet_chat_button, "clicked", G_CALLBACK(uchat_switch_current_chat), main_struct);
}
