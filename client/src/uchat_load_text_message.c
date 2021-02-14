#include "client.h"

void uchat_load_text_message(gint id, gchar *message) {
    GtkWidget *sended_message_box;
    GtkWidget *sended_message_label;
    GtkWidget *sended_time_stamp_label;

    sended_message_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(sended_message_box, "sended_message_box");
    gtk_widget_set_halign(sended_message_box, GTK_ALIGN_END);

    sended_message_label = gtk_label_new(message);
    gtk_widget_set_name(sended_message_label, "sended_message_label");
    gtk_widget_set_halign(sended_message_label, GTK_ALIGN_END);
    gtk_label_set_line_wrap(GTK_LABEL(sended_message_label), TRUE);
    gtk_label_set_selectable(GTK_LABEL(sended_message_label), TRUE);

    time_t timer = time(NULL);
    struct tm *time_info = localtime(&timer);
    gchar *time_stamp = (gchar *) malloc(6);
    strftime(time_stamp, 6, "%H:%M", time_info);

    sended_time_stamp_label = gtk_label_new(time_stamp);
    gtk_widget_set_name(sended_time_stamp_label, "sended_time_stamp_label");
    gtk_widget_set_halign(sended_time_stamp_label, GTK_ALIGN_END);

    gtk_box_pack_start(GTK_BOX(sended_message_box), sended_message_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sended_message_box), sended_time_stamp_label, FALSE, FALSE, 0);

    GList *chats = gtk_container_get_children(GTK_CONTAINER(main_struct->mainbar_scrolled_chat_box));

    while (chats) {
        GList *inner_data = gtk_container_get_children(GTK_CONTAINER(chats->data));

        if (!strcmp(user_list_get_user_login_by_id(main_struct->user_list, (guint) id), gtk_label_get_text(GTK_LABEL(inner_data->data)))) {
            inner_data = inner_data->next;

            gtk_box_pack_start(GTK_BOX(inner_data->data), sended_message_box, FALSE, FALSE, 0);

            if (!strcmp(main_struct->current->login, user_list_get_user_login_by_id(main_struct->user_list, id))) {
                gtk_widget_show_all(chats->data);
            }

            g_list_free(g_steal_pointer(&inner_data));

            break;
        }

        g_list_free(g_steal_pointer(&inner_data));

        chats = chats->next;
    }

    g_list_free(g_steal_pointer(&chats));

    pthread_t thread;
    pthread_create(&thread, NULL, uchat_mainbar_chat_scroll_thread, NULL);
}
