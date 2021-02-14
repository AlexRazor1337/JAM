#include "client.h"

void uchat_disconnect_close(t_main_struct *main_struct) {
    gtk_widget_hide(main_struct->disconnect_box);
    gtk_widget_show_all(main_struct->uchat_box);
}
