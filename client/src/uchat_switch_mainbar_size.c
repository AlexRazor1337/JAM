#include "client.h"

void uchat_switch_mainbar_size(GtkWidget *button, t_main_struct *main_struct) {
    if (!strcmp(gtk_widget_get_name(main_struct->mainbar_box), "mainbar_box")) {
        gtk_widget_hide(main_struct->stickerbar_box);
        gtk_widget_set_name(main_struct->mainbar_box, "mainbar_box_fullsize");
        gtk_widget_set_size_request(main_struct->mainbar_box, 1070, 795);
        gtk_widget_set_name(main_struct->mainbar_input_entry, "mainbar_input_box_input_entry_fullsize");
        gtk_widget_set_size_request(main_struct->mainbar_input_entry, 845, 75);
    } else {
        gtk_widget_show_all(main_struct->stickerbar_box);
        gtk_widget_set_name(main_struct->mainbar_box, "mainbar_box");
        gtk_widget_set_size_request(main_struct->mainbar_box, 780, 795);
        gtk_widget_set_name(main_struct->mainbar_input_entry, "mainbar_input_box_input_entry");
        gtk_widget_set_size_request(main_struct->mainbar_input_entry, 535, 75);
    }

    if (!button) return;
}
