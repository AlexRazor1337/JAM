#include "client.h"

void uchat_settings_set_image(GtkWidget *button, t_main_struct *main_struct) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Profile photo", GTK_WINDOW(main_struct->uchat_window), GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);
    gint run = gtk_dialog_run(GTK_DIALOG(dialog));

    if (run == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);

        gchar *source_path = gtk_file_chooser_get_filename(chooser);
        gchar *destination_path = NULL;
        gchar *binary_file_content = NULL;
        gchar *filename = source_path;

        while (strchr(filename, '/') != NULL) {
            filename = strchr(filename, '/');
            filename++;
        }

        destination_path = strjoin(destination_path, "resource/images/users/");
        destination_path = strjoin(destination_path, main_struct->auth->login);
        destination_path = strjoin(destination_path, ".png");

        FILE *source = fopen(source_path, "rb");
        FILE *destination = fopen(destination_path, "wb");

        fseek(source, 0, SEEK_END);
        gint length = ftell(source);
        fseek(source, 0, SEEK_SET);

        binary_file_content = (gchar *)malloc(length + 1);

        for (gint i = 0; i < length; i++) {
            binary_file_content[i] = fgetc(source);
            fputc(binary_file_content[i], destination);
        }

        binary_file_content[length] = '\0';

        fclose(source);
        fclose(destination);

        strdel(&main_struct->settings->image);
        main_struct->settings->image = strdup(destination_path);

        gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->settings->image_widget), gdk_pixbuf_new_from_file_at_scale(main_struct->settings->image, 125, 125, FALSE, NULL));

        strdel(&source_path);
        strdel(&destination_path);
    }

    gtk_widget_destroy(dialog);

    if (!button) return;
}
