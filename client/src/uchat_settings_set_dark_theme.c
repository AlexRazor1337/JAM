#include "client.h"

void uchat_settings_set_dark_theme(GtkWidget *button, t_main_struct *main_struct) {
    if (strcmp("dark", main_struct->theme)) {
        strdel(&main_struct->theme);
        main_struct->theme = strdup("dark");

        jamconfig_update_theme(main_struct->theme);
        uchat_settings_set_theme_provider(main_struct);

        gtk_image_set_from_file(GTK_IMAGE(main_struct->header_logo_image), "resource/images/dark/logo.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->panel_settings_image), "resource/images/dark/settings.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->panel_search_image), "resource/images/dark/search.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->panel_shop_image), "resource/images/dark/shop.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->panel_exit_image), "resource/images/dark/exit.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->mainbar_input_box_clip_image), "resource/images/dark/clip.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->mainbar_input_box_emoji_image), "resource/images/dark/emoji.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->mainbar_input_box_send_image), "resource/images/dark/send.png");
        gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->search_box_back_image), gdk_pixbuf_new_from_file_at_scale("resource/images/dark/close.png", 32, 32, TRUE, NULL));
        gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->search_box_add_image), gdk_pixbuf_new_from_file_at_scale("resource/images/dark/add.png", 32, 32, TRUE, NULL));
        if (gtk_entry_get_visibility(GTK_ENTRY(main_struct->settings->password_entry))) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/dark/eye_closed.png", 20, 20, TRUE, NULL));
        } else {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/dark/eye.png", 20, 20, TRUE, NULL));
        }
        gtk_image_set_from_file(GTK_IMAGE(main_struct->profile_image_set_image), "resource/images/dark/plus.png");
    }

    if (!button) return;
}
