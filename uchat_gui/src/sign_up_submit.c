#include "client.h"

void sign_up_submit(GtkWidget *button, t_main_struct *main_struct) {
    gtk_widget_hide(main_struct->login_is_empty);
    gtk_widget_hide(main_struct->login_is_too_short);
    gtk_widget_hide(main_struct->login_is_not_unique);

    gtk_widget_hide(main_struct->username_is_empty);
    gtk_widget_hide(main_struct->username_is_too_short);

    gtk_widget_hide(main_struct->password_is_empty);
    gtk_widget_hide(main_struct->password_is_too_short);

    gtk_widget_hide(main_struct->password_repeat_is_not_identical);

    if (strcmp(main_struct->auth->login, "")) {
        if (strlen(main_struct->auth->login) >= 4) {
            // check unique
            if (strcmp(main_struct->auth->username, "")) {
                if (strlen(main_struct->auth->username) >= 4) {
                    if (strcmp(main_struct->auth->password, "")) {
                        if (strlen(main_struct->auth->password) >= 6) {
                            if (strcmp(main_struct->auth->password_repeat, "")) {
                                if (!strcmp(main_struct->auth->password, main_struct->auth->password_repeat)) {
                                    main_struct->user_list = user_list_new(main_struct->auth->login, main_struct->auth->username);

                                    jamconfig_update_theme("default");
                                    uchat_settings_set_theme_provider(main_struct);

                                    client.login = main_struct->auth->login;
                                    client.password = main_struct->auth->password;

                                    t_connect_data *connect_data = (t_connect_data *)malloc(sizeof(t_connect_data));

                                    connect_data->thread = NULL;
                                    connect_data->to_sign_up = true;
                                    pthread_create(connect_data->thread, NULL, serverInit, connect_data);

                                    while (client.state == UNAUTH) {
                                        continue;
                                    }

                                    if (client.state == AUTH) {
                                        main_struct->user_list = user_list_new(main_struct->auth->login, main_struct->auth->username);

                                        // create chat
                                        uchat(NULL, main_struct);
                                    } else {
                                        g_print("SIGN UP FAILED\n");  // show label
                                    }
                                } else {
                                    gtk_widget_show_all(main_struct->password_repeat_is_not_identical);
                                }
                            } else {
                                gtk_widget_show_all(main_struct->password_repeat_is_not_identical);
                            }
                        } else {
                            gtk_widget_show_all(main_struct->password_is_too_short);
                        }
                    } else {
                        gtk_widget_show_all(main_struct->password_is_empty);
                    }
                } else {
                    gtk_widget_show_all(main_struct->username_is_too_short);
                }
            } else {
                gtk_widget_show_all(main_struct->username_is_empty);
            }
        } else {
            gtk_widget_show_all(main_struct->login_is_too_short);
        }
    } else {
        gtk_widget_show_all(main_struct->login_is_empty);
    }

    if (!button) return;
}
