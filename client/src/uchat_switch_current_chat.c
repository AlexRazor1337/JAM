#include "client.h"

void uchat_switch_current_chat(GtkWidget *button, t_main_struct *main_struct) {
    gtk_widget_set_name(main_struct->sidebar_currnet_chat_button, "sidebar_currnet_chat_button");
    gtk_widget_set_name(button, "sidebar_chat_focused");

    GList *fixed = gtk_container_get_children(GTK_CONTAINER(button));
    GList *elements = gtk_container_get_children(GTK_CONTAINER(fixed->data));
    main_struct->sidebar_currnet_chat_button = button;

    for (gint now = 0; elements; now++) {
        if (now == 0) {
            main_struct->sidebar_currnet_chat_image = GTK_WIDGET(elements->data);
        } else if (now == 1) {
            main_struct->sidebar_currnet_chat_username_label = GTK_WIDGET(elements->data);
        } else if (now == 2) {
            main_struct->sidebar_currnet_chat_login_label = GTK_WIDGET(elements->data);
        } else if (now == 3) {
            main_struct->sidebar_currnet_chat_status = GTK_WIDGET(elements->data);
        }

        elements = elements->next;
    }

    g_list_free(g_steal_pointer(&elements));
    g_list_free(g_steal_pointer(&fixed));

    gtk_image_set_from_pixbuf(GTK_IMAGE(main_struct->header_user_image), gtk_image_get_pixbuf(GTK_IMAGE(main_struct->sidebar_currnet_chat_image)));
    gtk_label_set_text(GTK_LABEL(main_struct->header_user_username_label), gtk_label_get_text(GTK_LABEL(main_struct->sidebar_currnet_chat_username_label)));
    gtk_label_set_text(GTK_LABEL(main_struct->header_user_login_label), gtk_label_get_text(GTK_LABEL(main_struct->sidebar_currnet_chat_login_label)));
    gtk_widget_set_name(main_struct->header_user_status, gtk_widget_get_name(main_struct->sidebar_currnet_chat_status));

    t_user_list *user_list_cpy = main_struct->user_list;

    while (user_list_cpy) {
        if (!strcmp(user_list_cpy->user->login, gtk_label_get_text(GTK_LABEL(main_struct->header_user_login_label)))) {
            main_struct->current->id = user_list_cpy->user->id;
            
            strdel(&main_struct->current->login);
            main_struct->current->login = strdup(user_list_cpy->user->login);

            strdel(&main_struct->current->username);
            main_struct->current->username = strdup(user_list_cpy->user->username);

            strdel(&main_struct->current->image);
            main_struct->current->image = strdup(user_list_cpy->user->image);

            break;
        }

        user_list_cpy = user_list_cpy->next;
    }

    gtk_entry_set_text(GTK_ENTRY(main_struct->mainbar_input_entry), "");

    uchat_show_current_chat(main_struct);
    uchat_mainbar_chat_scroll(main_struct);

    if (!button) return;
}
