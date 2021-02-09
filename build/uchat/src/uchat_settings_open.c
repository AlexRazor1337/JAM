#include "client.h"

void uchat_settings_open(GtkWidget *button, t_main_struct *main_struct) {
    gtk_widget_hide(main_struct->uchat_box);
    gtk_widget_show_all(main_struct->settings_box);

    gtk_entry_set_text(GTK_ENTRY(main_struct->settings->login_entry), main_struct->auth->login);
    gtk_entry_set_text(GTK_ENTRY(main_struct->settings->username_entry), main_struct->auth->username);
    gtk_entry_set_text(GTK_ENTRY(main_struct->settings->password_entry), main_struct->auth->password);

    strdel(&main_struct->settings->image);
    main_struct->settings->image = strdup(main_struct->auth->image);

    gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->settings->image_widget), gdk_pixbuf_new_from_file_at_scale(main_struct->settings->image, 125, 125, FALSE, NULL));

    if (gtk_entry_get_visibility(GTK_ENTRY(main_struct->settings->password_entry))) {
        gtk_entry_set_visibility(GTK_ENTRY(main_struct->settings->password_entry), FALSE);

        if (!strcmp(main_struct->theme, "default")) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/default/eye.png", 20, 20, TRUE, NULL));
        } else if (!strcmp(main_struct->theme, "light")) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/light/eye.png", 20, 20, TRUE, NULL));
        } else if (!strcmp(main_struct->theme, "dark")) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/dark/eye.png", 20, 20, TRUE, NULL));
        }
    }

    if (!button) return;
}
