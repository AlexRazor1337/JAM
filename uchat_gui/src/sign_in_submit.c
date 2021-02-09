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
                    if (client.state == AUTH_FAILED) {
                        client.state = UNAUTH;
                    }

                    strdel(&main_struct->auth->username);
                    main_struct->auth->username = strdup(main_struct->auth->login);

                    strdel(&main_struct->auth->password_repeat);
                    main_struct->auth->password_repeat = strdup(main_struct->auth->password);

                    client.login = main_struct->auth->login;
                    client.password = main_struct->auth->password;

                    pthread_t thread;
                    pthread_create(&thread, NULL, serverInit, &thread);

                    while (client.state == UNAUTH) {
                        continue;
                    }

                    if (client.state == AUTH) {
                        main_struct->user_list = user_list_new(main_struct->auth->login, main_struct->auth->username);

                        // TODO parse chats from server

                        // create chat
                        uchat(NULL, main_struct);
                    } else {
                        g_print("AUTH FAILED\n");  // show label
                    }
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
