#include "client.h"

void uchat_recieve_file_message(guint id, gchar *filename, gchar *path) {
    GtkWidget *recieved_message_box;
    GtkWidget *recieved_message_image;
    GtkWidget *recieved_time_stamp_label;

    recieved_message_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(recieved_message_box, "recieved_message_box");
    gtk_widget_set_halign(recieved_message_box, GTK_ALIGN_START);

    recieved_message_image = gtk_image_new();
    gtk_widget_set_name(recieved_message_image, "recieved_message_image");
    gtk_widget_set_tooltip_text(recieved_message_image, filename);
    gtk_widget_set_halign(recieved_message_image, GTK_ALIGN_START);

    time_t timer = time(NULL);
    struct tm *time_info = localtime(&timer);
    gchar *time_stamp = (gchar *) malloc(6);
    strftime(time_stamp, 6, "%H:%M", time_info);

    recieved_time_stamp_label = gtk_label_new(time_stamp);
    gtk_widget_set_name(recieved_time_stamp_label, "recieved_time_stamp_label");
    gtk_widget_set_halign(recieved_time_stamp_label, GTK_ALIGN_START);

    if (!strcmp(strchr(filename, '.'), ".png") || !strcmp(strchr(filename, '.'), ".jpg") || !strcmp(strchr(filename, '.'), ".jpeg")) {
        GdkPixbuf *message_file_pixbuf = gdk_pixbuf_new_from_file(path, NULL);
        gint width = gdk_pixbuf_get_width(message_file_pixbuf);
        gint height = gdk_pixbuf_get_height(message_file_pixbuf);

        if (width > 400 || height > 400) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(recieved_message_image), gdk_pixbuf_new_from_file_at_scale(path, 400, 400, TRUE, NULL));
        } else {
            gtk_image_set_from_pixbuf(GTK_IMAGE(recieved_message_image), message_file_pixbuf);
        }

        g_object_unref(G_OBJECT(message_file_pixbuf));
    } else if (!strcmp(strchr(filename, '.'), ".gif")) {
        GdkPixbufAnimation *message_file_pixbuf_animation = gdk_pixbuf_animation_new_from_file(path, NULL);
        gtk_image_set_from_animation(GTK_IMAGE(recieved_message_image), message_file_pixbuf_animation);
        g_object_unref(G_OBJECT(message_file_pixbuf_animation));
    } else {
        gtk_image_set_from_pixbuf(GTK_IMAGE(recieved_message_image), gdk_pixbuf_new_from_file_at_scale("resource/images/file.png", 90, 90, TRUE, NULL));
    }

    gtk_box_pack_start(GTK_BOX(recieved_message_box), recieved_message_image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(recieved_message_box), recieved_time_stamp_label, FALSE, FALSE, 0);

    GList *chats = gtk_container_get_children(GTK_CONTAINER(main_struct->mainbar_scrolled_chat_box));

    while (chats) {
        GList *inner_data = gtk_container_get_children(GTK_CONTAINER(chats->data));

        if (!strcmp(user_list_get_user_login_by_id(main_struct->user_list, id), gtk_label_get_text(GTK_LABEL(inner_data->data)))) {
            inner_data = inner_data->next;

            gtk_box_pack_start(GTK_BOX(inner_data->data), recieved_message_box, FALSE, FALSE, 0);

            system("afplay resource/audio/receive-file.mp3");

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

    g_print("Recieved from %s(%d) to %s(%d) sticker message: %s\n", main_struct->current->username, main_struct->current->id, main_struct->auth->username, main_struct->auth->id, filename);

    if (!strcmp(main_struct->current->login, user_list_get_user_login_by_id(main_struct->user_list, id))) {
        pthread_t thread;
        pthread_create(&thread, NULL, uchat_mainbar_chat_scroll_thread, NULL);
    }
}

