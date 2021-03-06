#include "client.h"

void uchat_send_file_message(GtkWidget *button, t_main_struct *main_struct) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("User image", GTK_WINDOW(main_struct->uchat_window), GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);
    gint run = gtk_dialog_run(GTK_DIALOG(dialog));

    if (run == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);

        gchar *path = gtk_file_chooser_get_filename(chooser);
        gchar *filename = path;

        while (strchr(filename, '/') != NULL) {
            filename = strchr(filename, '/');
            filename++;
        }

        FILE *source = fopen(path, "rb");

        GtkWidget *sended_message_box;
        GtkWidget *sended_message_image;
        GtkWidget *sended_time_stamp_label;

        sended_message_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_name(sended_message_box, "sended_message_box");
        gtk_widget_set_halign(sended_message_box, GTK_ALIGN_END);

        sended_message_image = gtk_image_new();
        gtk_widget_set_name(sended_message_image, "sended_message_image");
        gtk_widget_set_tooltip_text(sended_message_image, filename);
        gtk_widget_set_halign(sended_message_image, GTK_ALIGN_END);

        time_t timer = time(NULL);
        struct tm *time_info = localtime(&timer);
        gchar *time_stamp = (gchar *) malloc(6);
        strftime(time_stamp, 6, "%H:%M", time_info);

        sended_time_stamp_label = gtk_label_new(time_stamp);
        gtk_widget_set_name(sended_time_stamp_label, "sended_time_stamp_label");
        gtk_widget_set_halign(sended_time_stamp_label, GTK_ALIGN_END);

        if (!strcmp(strchr(filename, '.'), ".png") || !strcmp(strchr(filename, '.'), ".jpg") || !strcmp(strchr(filename, '.'), ".jpeg")) {
            GdkPixbuf *message_file_pixbuf = gdk_pixbuf_new_from_file(path, NULL);
            gint width = gdk_pixbuf_get_width(message_file_pixbuf);
            gint height = gdk_pixbuf_get_height(message_file_pixbuf);

            if (width > 400 || height > 400) {
                gtk_image_set_from_pixbuf(GTK_IMAGE(sended_message_image), gdk_pixbuf_new_from_file_at_scale(path, 400, 400, TRUE, NULL));
            } else {
                gtk_image_set_from_pixbuf(GTK_IMAGE(sended_message_image), message_file_pixbuf);
            }

            g_object_unref(G_OBJECT(message_file_pixbuf));
        } else if (!strcmp(strchr(filename, '.'), ".gif")) {
            GdkPixbufAnimation *message_file_pixbuf_animation = gdk_pixbuf_animation_new_from_file(path, NULL);
            gtk_image_set_from_animation(GTK_IMAGE(sended_message_image), message_file_pixbuf_animation);
            g_object_unref(G_OBJECT(message_file_pixbuf_animation));
        } else {
            gtk_image_set_from_pixbuf(GTK_IMAGE(sended_message_image), gdk_pixbuf_new_from_file_at_scale("resource/images/file.png", 90, 90, TRUE, NULL));
        }

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

        system("afplay resource/audio/send.mp3");

        // sendFileMessage(main_struct->current->id, filename, path, source);

        uchat_mainbar_chat_scroll(main_struct);
        uchat_send_text_message(NULL, main_struct);

        strdel(&path);
        fclose(source);
    }

    gtk_widget_destroy(dialog);

    if (!button) return;
}
