#include "client.h"

void uchat_disconnect_open(t_main_struct *main_struct) {
    gtk_widget_hide(main_struct->uchat_box);
    gtk_widget_hide(main_struct->search_box);
    gtk_widget_hide(main_struct->settings_box)
    gtk_widget_show_all(main_struct->disconnect_box);

    if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_file(GTK_IMAGE(main_struct->disconnect->disconnect_logo_image), "resource/images/default/jam.png");
    } else if (!strcmp(main_struct->theme, "light")) {
        gtk_image_set_from_file(GTK_IMAGE(main_struct->disconnect->disconnect_logo_image), "resource/images/light/jam.png");
    } else if (!strcmp(main_struct->theme, "dark")) {
        gtk_image_set_from_file(GTK_IMAGE(main_struct->disconnect->disconnect_logo_image), "resource/images/dark/jam.png");
    }
}
