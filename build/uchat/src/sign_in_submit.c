#include "client.h"

void sign_in_submit(GtkWidget *button, t_main_struct *main_struct) {
    gtk_widget_hide(main_struct->login_is_empty);
    gtk_widget_hide(main_struct->login_is_too_short);
    gtk_widget_hide(main_struct->login_is_not_unique);

    gtk_widget_hide(main_struct->password_is_empty);
    gtk_widget_hide(main_struct->password_is_too_short);

    if (strcmp(main_struct->auth->login, "")) {
        if (strlen(main_struct->auth->login) >= 4) {
            if (strcmp(main_struct->auth->password, "")) {
                if (strlen(main_struct->auth->password) >= 6) {
                    // check if this user exist with this data

                    // get from server
                    strdel(&main_struct->auth->username);
                    main_struct->auth->username = strdup(main_struct->auth->login);

                    strdel(&main_struct->auth->password_repeat);
                    main_struct->auth->password_repeat = strdup(main_struct->auth->password);

                    // if user exist then load his image
                    main_struct->user_list = user_list_new(main_struct->auth->login, main_struct->auth->username);

                    // parse users from server

                    // parse imitation
                    user_list_push(&main_struct->user_list, "kili", "Liza");
                    user_list_push(&main_struct->user_list, "Yana", "Yana");
                    user_list_push(&main_struct->user_list, "vchkhr", "Slava");
                    user_list_push(&main_struct->user_list, "AlexRazor", "Alex");
                    user_list_push(&main_struct->user_list, "PAXANDOS", "Pasha");
                    // user_list_print(main_struct->user_list);

                    // create chat
                    uchat(NULL, main_struct);
                } else {
                    gtk_widget_show_all(main_struct->password_is_too_short);
                }
            } else {
                gtk_widget_show_all(main_struct->password_is_empty);
            }
        } else {
            gtk_widget_show_all(main_struct->login_is_too_short);
        }
    } else {
        gtk_widget_show_all(main_struct->login_is_empty);
    }

    if (!button) return;
}
