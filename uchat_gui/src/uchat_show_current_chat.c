#include "client.h"

void uchat_show_current_chat(t_main_struct *main_struct) {
    GList *fixed_children = gtk_container_get_children(GTK_CONTAINER(main_struct->mainbar_scrolled_chat_box));

    while (fixed_children) {
        gtk_widget_hide(fixed_children->data);

        fixed_children = fixed_children->next;
    }

    g_list_free(g_steal_pointer(&fixed_children));

    fixed_children = gtk_container_get_children(GTK_CONTAINER(main_struct->mainbar_scrolled_chat_box));

    while (fixed_children) {
        GList *fixed_inner = gtk_container_get_children(GTK_CONTAINER(fixed_children->data));

        if (!strcmp(gtk_label_get_text(GTK_LABEL(main_struct->sidebar_currnet_chat_login_label)), gtk_label_get_text(GTK_LABEL(fixed_inner->data)))) {
            gtk_widget_show_all(fixed_children->data);

            g_list_free(g_steal_pointer(&fixed_inner));

            break;
        }

        g_list_free(g_steal_pointer(&fixed_inner));

        fixed_children = fixed_children->next;
    }

    g_list_free(g_steal_pointer(&fixed_children));
}
