#include "client.h"

void uchat_settings_close_with_save(GtkWidget *button, t_main_struct *main_struct) {
    gboolean error = FALSE;
    gboolean is_changed = FALSE;

    // save data
    // update username
    if (strcmp(main_struct->auth->username, (gchar *)gtk_entry_get_text(GTK_ENTRY(main_struct->settings->username_entry)))) {
        if (strlen((gchar *)gtk_entry_get_text(GTK_ENTRY(main_struct->settings->username_entry))) > 3) {
            strdel(&main_struct->auth->username);
            main_struct->auth->username = strdup((gchar *)gtk_entry_get_text(GTK_ENTRY(main_struct->settings->username_entry)));

            gtk_label_set_text(GTK_LABEL(main_struct->sidebar_self_chat_username_label), main_struct->auth->username);

            if (!strcmp(main_struct->auth->login, (gchar *)gtk_label_get_text(GTK_LABEL(main_struct->header_user_login_label)))) {
                gtk_label_set_text(GTK_LABEL(main_struct->header_user_username_label), main_struct->auth->username);

                strdel(&main_struct->current->username);
                main_struct->current->username = strdup((gchar *)gtk_entry_get_text(GTK_ENTRY(main_struct->settings->username_entry)));
            }

            // update username on server
            is_changed = TRUE;
        } else {
            error = TRUE;
        }
    }
    // update password
    if (strcmp(main_struct->auth->password, (gchar *)gtk_entry_get_text(GTK_ENTRY(main_struct->settings->password_entry)))) {
        if (strlen((gchar *)gtk_entry_get_text(GTK_ENTRY(main_struct->settings->password_entry))) > 5) {
            strdel(&main_struct->auth->password);

            main_struct->auth->password = strdup((gchar *)gtk_entry_get_text(GTK_ENTRY(main_struct->settings->password_entry)));

            strdel(&main_struct->auth->password_repeat);
            main_struct->auth->password_repeat = strdup((gchar *)gtk_entry_get_text(GTK_ENTRY(main_struct->settings->password_entry)));

            // update password on server
            is_changed = TRUE;
        } else {
            error = TRUE;
        }
    }
    // update image
    if (strcmp(main_struct->auth->image, main_struct->settings->image)) {
        strdel(&main_struct->auth->image);
        main_struct->auth->image = strdup(main_struct->settings->image);

        gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->sidebar_self_chat_image), gdk_pixbuf_new_from_file_at_scale(main_struct->auth->image, 50, 50, FALSE, NULL));

        if (!strcmp(main_struct->auth->login, (gchar *)gtk_label_get_text(GTK_LABEL(main_struct->header_user_login_label)))) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->header_user_image), gdk_pixbuf_new_from_file_at_scale(main_struct->auth->image, 50, 50, FALSE, NULL));
        
            strdel(&main_struct->current->image);
            main_struct->current->image = strdup(main_struct->settings->image);
        }

        // update image on server
        // oAoAoAo images
    }

    if (!error) {
        gtk_widget_hide(main_struct->settings_box);
        gtk_widget_show_all(main_struct->uchat_box);

        uchat_show_current_chat(main_struct);
        uchat_switch_mainbar_size(NULL, main_struct);
        uchat_switch_mainbar_size(NULL, main_struct);

        gtk_widget_hide(gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(main_struct->sidebar_scrolled_chats)));
        gtk_widget_hide(gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(main_struct->mainbar_scrolled_chat)));
        gtk_widget_hide(gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(main_struct->stickerbar_scrolled)));
    
        if (is_changed) {
            updateCredentials(main_struct->auth->username, main_struct->auth->password);
        }
    }

    if (!button) return;
}
