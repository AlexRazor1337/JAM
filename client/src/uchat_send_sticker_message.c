#include "client.h"

void uchat_send_sticker_message(GtkWidget *button, t_main_struct *main_struct) {
    GList *fixed_child = gtk_container_get_children(GTK_CONTAINER(button));
    GList *fixed_inner = gtk_container_get_children(GTK_CONTAINER(fixed_child->data));

    gchar *stickername = (gchar *)gtk_label_get_text(GTK_LABEL(fixed_inner->data));
    fixed_inner = fixed_inner->next;

    GtkWidget *sended_message_box;
    GtkWidget *sended_message_image;
    GtkWidget *sended_time_stamp_label;

    sended_message_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(sended_message_box, "sended_message_box");
    gtk_widget_set_halign(sended_message_box, GTK_ALIGN_END);

    sended_message_image = gtk_image_new_from_pixbuf(gtk_image_get_pixbuf(GTK_IMAGE(fixed_inner->data)));
    gtk_widget_set_name(sended_message_image, "sended_message_image");
    gtk_widget_set_halign(sended_message_image, GTK_ALIGN_END);

    time_t timer = time(NULL);
    struct tm *time_info = localtime(&timer);
    gchar *time_stamp = (gchar *) malloc(6);
    strftime(time_stamp, 6, "%H:%M", time_info);

    sended_time_stamp_label = gtk_label_new(time_stamp);
    gtk_widget_set_name(sended_time_stamp_label, "sended_time_stamp_label");
    gtk_widget_set_halign(sended_time_stamp_label, GTK_ALIGN_END);

    gtk_box_pack_start(GTK_BOX(sended_message_box), sended_message_image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sended_message_box), sended_time_stamp_label, FALSE, FALSE, 0);

    GList *chats = gtk_container_get_children(GTK_CONTAINER(main_struct->mainbar_scrolled_chat_box));

    while (chats) {
        GList *inner_data = gtk_container_get_children(GTK_CONTAINER(chats->data));

        if (!strcmp(gtk_label_get_text(GTK_LABEL(main_struct->sidebar_currnet_chat_login_label)), gtk_label_get_text(GTK_LABEL(inner_data->data)))) {
            inner_data = inner_data->next;

            gtk_box_pack_start(GTK_BOX(inner_data->data), sended_message_box, FALSE, FALSE, 0);

            gtk_widget_show_all(chats->data);

            g_list_free(g_steal_pointer(&inner_data));

            break;
        }

        g_list_free(g_steal_pointer(&inner_data));

        chats = chats->next;
    }

    g_list_free(g_steal_pointer(&chats));
    g_list_free(g_steal_pointer(&fixed_inner));
    g_list_free(g_steal_pointer(&fixed_child));

    system("afplay resource/audio/send.mp3");

    sendMessage(main_struct->current->id, stickername, 1);

    uchat_mainbar_chat_scroll(main_struct);

    if (!button) return;
}
