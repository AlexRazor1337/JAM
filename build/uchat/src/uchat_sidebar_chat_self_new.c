#include "client.h"

void uchat_sidebar_chat_self_new(t_main_struct *main_struct) {
    GtkWidget *sidebar_self_chat_button;
    GtkWidget *sidebar_self_chat_fixed;
    GtkWidget *sidebar_self_chat_image;
    GtkWidget *sidebar_self_chat_username_label;
    GtkWidget *sidebar_self_chat_login_label;
    GtkWidget *sidebar_self_chat_status;

    sidebar_self_chat_button = gtk_button_new();
    gtk_widget_set_name(sidebar_self_chat_button, "sidebar_self_chat_button");
    gtk_widget_set_name(sidebar_self_chat_button, "sidebar_chat_focused");
    gtk_widget_set_size_request(sidebar_self_chat_button, 260, 65);

    sidebar_self_chat_fixed = gtk_fixed_new();
    gtk_widget_set_name(sidebar_self_chat_fixed, "sidebar_self_chat_fixed");
    gtk_widget_set_size_request(sidebar_self_chat_fixed, 260, 65);

    sidebar_self_chat_image = gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_file_at_scale(main_struct->auth->image, 50, 50, FALSE, NULL));  // scale
    gtk_widget_set_name(sidebar_self_chat_image, "sidebar_self_chat_image");
    gtk_widget_set_size_request(sidebar_self_chat_image, 65, 65);

    sidebar_self_chat_username_label = gtk_label_new(main_struct->auth->username);
    gtk_widget_set_name(sidebar_self_chat_username_label, "sidebar_self_chat_username_label");

    sidebar_self_chat_login_label = gtk_label_new(main_struct->auth->login);
    gtk_widget_set_name(sidebar_self_chat_login_label, "sidebar_self_chat_login_label");

    sidebar_self_chat_status = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(sidebar_self_chat_status, "online");
    gtk_widget_set_size_request(sidebar_self_chat_status, 10, 10);

    gtk_fixed_put(GTK_FIXED(sidebar_self_chat_fixed), sidebar_self_chat_image, 0, 0);
    gtk_fixed_put(GTK_FIXED(sidebar_self_chat_fixed), sidebar_self_chat_username_label, 65, 10);
    gtk_fixed_put(GTK_FIXED(sidebar_self_chat_fixed), sidebar_self_chat_login_label, 65, 35);
    gtk_fixed_put(GTK_FIXED(sidebar_self_chat_fixed), sidebar_self_chat_status, 50, 50);

    gtk_container_add(GTK_CONTAINER(sidebar_self_chat_button), sidebar_self_chat_fixed);
    gtk_box_pack_start(GTK_BOX(main_struct->sidebar_scrolled_chats_box), sidebar_self_chat_button, FALSE, FALSE, 0);

    g_signal_connect(sidebar_self_chat_button, "clicked", G_CALLBACK(uchat_switch_current_chat), main_struct);

    main_struct->sidebar_self_chat_image = sidebar_self_chat_image;
    main_struct->sidebar_self_chat_username_label = sidebar_self_chat_username_label;
    main_struct->sidebar_self_chat_login_label = sidebar_self_chat_login_label;

    main_struct->sidebar_currnet_chat_button = sidebar_self_chat_button;
    main_struct->sidebar_currnet_chat_image = sidebar_self_chat_image;
    main_struct->sidebar_currnet_chat_username_label = sidebar_self_chat_username_label;
    main_struct->sidebar_currnet_chat_login_label = sidebar_self_chat_login_label;
    main_struct->sidebar_currnet_chat_status = sidebar_self_chat_status;
}
