#include "client.h"

void uchat_recieve_text_message(guint id, gchar *message) {
    GtkWidget *recieved_message_box;
    GtkWidget *recieved_message_label;
    GtkWidget *recieved_time_stamp_label;

    recieved_message_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(recieved_message_box, "recieved_message_box");
    gtk_widget_set_halign(recieved_message_box, GTK_ALIGN_START);

    recieved_message_label = gtk_label_new(message);
    gtk_widget_set_name(recieved_message_label, "recieved_message_label");
    gtk_widget_set_halign(recieved_message_label, GTK_ALIGN_START);
    gtk_label_set_line_wrap(GTK_LABEL(recieved_message_label), TRUE);
    gtk_label_set_selectable(GTK_LABEL(recieved_message_label), TRUE);

    time_t timer = time(NULL);
    struct tm *time_info = localtime(&timer);
    gchar *time_stamp = (gchar *) malloc(6);
    strftime(time_stamp, 6, "%H:%M", time_info);

    recieved_time_stamp_label = gtk_label_new(time_stamp);
    gtk_widget_set_name(recieved_time_stamp_label, "recieved_time_stamp_label");
    gtk_widget_set_halign(recieved_time_stamp_label, GTK_ALIGN_START);

    gtk_box_pack_start(GTK_BOX(recieved_message_box), recieved_message_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(recieved_message_box), recieved_time_stamp_label, FALSE, FALSE, 0);

    GList *chats = gtk_container_get_children(GTK_CONTAINER(main_struct->mainbar_scrolled_chat_box));

    while (chats) {
        GList *inner_data = gtk_container_get_children(GTK_CONTAINER(chats->data));
        if (!strcmp(user_list_get_user_login_by_id(main_struct->user_list, id), gtk_label_get_text(GTK_LABEL(inner_data->data)))) {
            inner_data = inner_data->next;

            gtk_box_pack_start(GTK_BOX(inner_data->data), recieved_message_box, FALSE, FALSE, 0);

            system("afplay resource/audio/receive-text.mp3");

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

    g_print("Recieved from %s(%d) to %s(%d) text message: %s\n", main_struct->current->username, main_struct->current->id, main_struct->auth->username, main_struct->auth->id, message);

    if (!strcmp(main_struct->current->login, user_list_get_user_login_by_id(main_struct->user_list, id))) {
        pthread_t thread;
        pthread_create(&thread, NULL, uchat_mainbar_chat_scroll_thread, NULL);
    }
}
