#include "client.h"

void uchat_settings_toggle_password_visibility(GtkWidget *button, t_main_struct *main_struct) {
    if (gtk_entry_get_visibility(GTK_ENTRY(main_struct->settings->password_entry))) {
        gtk_entry_set_visibility(GTK_ENTRY(main_struct->settings->password_entry), FALSE);

        if (!strcmp(main_struct->theme, "default")) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/default/eye.png", 20, 20, TRUE, NULL));
        } else if (!strcmp(main_struct->theme, "light")) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/light/eye.png", 20, 20, TRUE, NULL));
        } else if (!strcmp(main_struct->theme, "dark")) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/dark/eye.png", 20, 20, TRUE, NULL));
        }
    } else {
        gtk_entry_set_visibility(GTK_ENTRY(main_struct->settings->password_entry), TRUE);

        if (!strcmp(main_struct->theme, "default")) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/default/eye_closed.png", 20, 20, TRUE, NULL));
        } else if (!strcmp(main_struct->theme, "light")) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/light/eye_closed.png", 20, 20, TRUE, NULL));
        } else if (!strcmp(main_struct->theme, "dark")) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/dark/eye_closed.png", 20, 20, TRUE, NULL));
        }
    }

    if (!button) return;
}
