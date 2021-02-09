#include "client.h"

void uchat_settings_set_light_theme(GtkWidget *button, t_main_struct *main_struct) {
    if (strcmp("light", main_struct->theme)) {
        strdel(&main_struct->theme);
        main_struct->theme = strdup("light");

        jamconfig_update_theme(main_struct->theme);
        uchat_settings_set_theme_provider(main_struct);

        gtk_image_set_from_file(GTK_IMAGE(main_struct->header_logo_image), "resource/images/light/logo.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->panel_settings_image), "resource/images/light/settings.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->panel_search_image), "resource/images/light/search.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->panel_shop_image), "resource/images/light/shop.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->panel_exit_image), "resource/images/light/exit.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->mainbar_input_box_clip_image), "resource/images/light/clip.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->mainbar_input_box_emoji_image), "resource/images/light/emoji.png");
        gtk_image_set_from_file(GTK_IMAGE(main_struct->mainbar_input_box_send_image), "resource/images/light/send.png");
        gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->search_box_back_image), gdk_pixbuf_new_from_file_at_scale("resource/images/light/close.png", 32, 32, TRUE, NULL));
        gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->search_box_add_image), gdk_pixbuf_new_from_file_at_scale("resource/images/light/add.png", 32, 32, TRUE, NULL));
        if (gtk_entry_get_visibility(GTK_ENTRY(main_struct->settings->password_entry))) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/light/eye_closed.png", 20, 20, TRUE, NULL));
        } else {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/light/eye.png", 20, 20, TRUE, NULL));
        }
        gtk_image_set_from_file(GTK_IMAGE(main_struct->profile_image_set_image), "resource/images/light/plus.png");
    }

    if (!button) return;
}
