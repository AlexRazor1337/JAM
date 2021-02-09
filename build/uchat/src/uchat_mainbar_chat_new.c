#include "client.h"

void uchat_mainbar_chat_new(t_main_struct *main_struct, t_user *current) {
    GtkWidget *mainbar_inner_chat_box;
    GtkWidget *mainbar_inner_call_label;
    GtkWidget *mainbar_inner_message_box;

    mainbar_inner_chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(mainbar_inner_chat_box, "mainbar_inner_chat_box");
    gtk_widget_set_size_request(mainbar_inner_chat_box, 720, 50);

    mainbar_inner_call_label = gtk_label_new(current->login);
    gtk_widget_set_name(mainbar_inner_call_label, "mainbar_inner_call_label");

    mainbar_inner_message_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(mainbar_inner_message_box, "mainbar_inner_message_box");
    gtk_widget_set_halign(mainbar_inner_message_box, GTK_ALIGN_END);

    gtk_box_pack_start(GTK_BOX(mainbar_inner_chat_box), mainbar_inner_call_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mainbar_inner_chat_box), mainbar_inner_message_box, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(main_struct->mainbar_scrolled_chat_box), mainbar_inner_chat_box, FALSE, FALSE, 0);
}
