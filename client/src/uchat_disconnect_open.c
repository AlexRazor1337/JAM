#include "client.h"

void uchat_disconnect_open(t_main_struct *main_struct) {
    gtk_widget_hide(main_struct->uchat_box);
    gtk_widget_show_all(main_struct->disconnect_box);

    // gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->disconnect->image_widget), gdk_pixbuf_new_from_file_at_scale(main_struct->disconnect->image, 125, 125, FALSE, NULL));

    // if (!strcmp(main_struct->theme, "default")) {
    //     gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/default/eye.png", 20, 20, TRUE, NULL));
    // } else if (!strcmp(main_struct->theme, "light")) {
    //     gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/light/eye.png", 20, 20, TRUE, NULL));
    // } else if (!strcmp(main_struct->theme, "dark")) {
    //     gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/dark/eye.png", 20, 20, TRUE, NULL));
    // }
}
