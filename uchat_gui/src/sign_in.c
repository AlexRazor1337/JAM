#include "client.h"

void sign_in(GtkWidget *button, t_main_struct *main_struct) {
    // init
    GtkWidget *sign_in_window;
    GtkWidget *sign_in_box;

    GtkWidget *logo_box;
    GtkWidget *logo_image;
    GtkWidget *sign_box;

    GtkWidget *login_fixed;
    GtkWidget *login_entry;
    GtkWidget *login_is_empty;
    GtkWidget *login_is_too_short;
    GtkWidget *login_is_not_unique;

    GtkWidget *password_fixed;
    GtkWidget *password_entry;
    GtkWidget *password_is_empty;
    GtkWidget *password_is_too_short;

    GtkWidget *control_box;
    GtkWidget *submit;
    GtkWidget *unsubmit;

    // window
    sign_in_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(sign_in_window), "JAM Sign in");
    gtk_window_set_resizable(GTK_WINDOW(sign_in_window), FALSE);
    gtk_window_set_position(GTK_WINDOW(sign_in_window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(sign_in_window), 1400, 900);

    // main box
    sign_in_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(sign_in_box, "sign_in_box");

    // logo box
    logo_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(logo_box, "logo_box");

    // logo image
    logo_image = gtk_image_new_from_file("resource/images/sign_up_in_logo.png");
    gtk_widget_set_name(logo_image, "logo_image");

    // main sign box
    sign_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(sign_box, "sign_box");
    gtk_widget_set_halign(sign_box, GTK_ALIGN_CENTER);

    // login fixed
    login_fixed = gtk_fixed_new();
    gtk_widget_set_name(login_fixed, "login_fixed");

    // login entry
    login_entry = gtk_entry_new();
    gtk_widget_set_name(login_entry, "login_entry");
    gtk_widget_set_size_request(login_entry, 400, 40);
    gtk_entry_set_placeholder_text(GTK_ENTRY(login_entry), "LOGIN");
    gtk_entry_set_max_length(GTK_ENTRY(login_entry), 16);

    // login is empty -> error label
    login_is_empty = gtk_label_new("LOGIN INPUT IS EMPTY");
    gtk_widget_set_name(login_is_empty, "login_is_empty");
    gtk_widget_set_size_request(login_is_empty, 400, 40);

    // login is too short -> error label
    login_is_too_short = gtk_label_new("LOGIN IS TOO SHORT");
    gtk_widget_set_name(login_is_too_short, "login_is_too_short");
    gtk_widget_set_size_request(login_is_too_short, 400, 40);

    // login is not unique -> error label
    login_is_not_unique = gtk_label_new("LOGIN IS NOT UNIQUE");
    gtk_widget_set_name(login_is_not_unique, "login_is_not_unique");
    gtk_widget_set_size_request(login_is_not_unique, 400, 40);

    // password fixed
    password_fixed = gtk_fixed_new();
    gtk_widget_set_name(password_fixed, "password_fixed");

    // password entry
    password_entry = gtk_entry_new();
    gtk_widget_set_name(password_entry, "password_entry");
    gtk_widget_set_size_request(password_entry, 400, 40);
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "PASSWORD");
    gtk_entry_set_max_length(GTK_ENTRY(password_entry), 16);
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    // username is empty -> error label
    password_is_empty = gtk_label_new("PASSWORD INPUT IS EMPTY");
    gtk_widget_set_name(password_is_empty, "password_is_empty");
    gtk_widget_set_size_request(password_is_empty, 400, 40);

    // password is too short -> error label
    password_is_too_short = gtk_label_new("PASSWORD IS TOO SHORT");
    gtk_widget_set_name(password_is_too_short, "password_is_too_short");
    gtk_widget_set_size_request(password_is_too_short, 400, 40);

    // control box for submit and unsubmit
    control_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(control_box, "control_box");

    // unsubmit button
    unsubmit = gtk_button_new_with_label("BACK");
    gtk_widget_set_name(unsubmit, "unsubmit");
    gtk_widget_set_size_request(unsubmit, 200, 40);

    // submit button
    submit = gtk_button_new_with_label("SIGN IN");
    gtk_widget_set_name(submit, "submit");
    gtk_widget_set_size_request(submit, 200, 40);

    // packing
    gtk_box_pack_start(GTK_BOX(logo_box), logo_image, FALSE, FALSE, 0);

    gtk_fixed_put(GTK_FIXED(login_fixed), login_entry, 0, 0);
    gtk_fixed_put(GTK_FIXED(login_fixed), login_is_empty, 0, 54);
    gtk_fixed_put(GTK_FIXED(login_fixed), login_is_too_short, 0, 54);
    gtk_fixed_put(GTK_FIXED(login_fixed), login_is_not_unique, 0, 54);

    gtk_fixed_put(GTK_FIXED(password_fixed), password_entry, 0, 0);
    gtk_fixed_put(GTK_FIXED(password_fixed), password_is_empty, 0, 54);
    gtk_fixed_put(GTK_FIXED(password_fixed), password_is_too_short, 0, 54);

    gtk_box_pack_start(GTK_BOX(control_box), unsubmit, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(control_box), submit, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(sign_box), login_fixed, FALSE, FALSE, 7);
    gtk_box_pack_start(GTK_BOX(sign_box), password_fixed, FALSE, FALSE, 7);
    gtk_box_pack_start(GTK_BOX(sign_box), control_box, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(sign_in_box), logo_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sign_in_box), sign_box, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(sign_in_window), sign_in_box);

    // signals
    g_signal_connect(login_entry, "changed", G_CALLBACK(sign_in_update_login), main_struct);
    g_signal_connect(password_entry, "changed", G_CALLBACK(sign_in_update_password), main_struct);

    g_signal_connect(login_entry, "activate", G_CALLBACK(sign_in_submit), main_struct);
    g_signal_connect(password_entry, "activate", G_CALLBACK(sign_in_submit), main_struct);

    g_signal_connect(unsubmit, "clicked", G_CALLBACK(authorization), main_struct);
    g_signal_connect(submit, "clicked", G_CALLBACK(sign_in_submit), main_struct);

    g_signal_connect(sign_in_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // show
    gtk_widget_hide(main_struct->authorization_window);

    gtk_widget_show(sign_in_window);
    gtk_widget_show(sign_in_box);

    gtk_widget_show(logo_box);
    gtk_widget_show(sign_box);

    gtk_widget_show(logo_image);

    gtk_widget_show(login_fixed);
    gtk_widget_show(password_fixed);
    gtk_widget_show(control_box);

    gtk_widget_show(login_entry);
    gtk_widget_show(password_entry);
    gtk_widget_show(unsubmit);
    gtk_widget_show(submit);

    // init struct
    main_struct->sign_in_window = sign_in_window;

    main_struct->login_is_empty = login_is_empty;
    main_struct->login_is_too_short = login_is_too_short;
    main_struct->login_is_not_unique = login_is_not_unique;

    main_struct->password_is_empty = password_is_empty;
    main_struct->password_is_too_short = password_is_too_short;

    strdel(&main_struct->auth->login);
    main_struct->auth->login = strdup("");

    strdel(&main_struct->auth->password);
    main_struct->auth->password = strdup("");

    // plug
    if (!button) return;
}
