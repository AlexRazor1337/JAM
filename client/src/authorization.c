#include "client.h"

void authorization(GtkWidget *button, t_main_struct *main_struct) {
    // init
    GtkWidget *authorization_window;
    GtkWidget *authorization_box;
    GtkWidget *logo_box;
    GtkWidget *logo_image;
    GtkWidget *sign_up_in_box;
    GtkWidget *sign_up_button;
    GtkWidget *sign_in_button;

    // window
    authorization_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(authorization_window), "JAM Authorization");
    gtk_window_set_resizable(GTK_WINDOW(authorization_window), FALSE);
    gtk_window_set_position(GTK_WINDOW(authorization_window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(authorization_window), 1400, 900);

    // main box
    authorization_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(authorization_box, "authorization_box");

    // logo box
    logo_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_name(logo_box, "logo_box");

    // logo image
    logo_image = gtk_image_new_from_file("resource/images/auth_logo.png");
    gtk_widget_set_name(logo_image, "logo_image");

    // sign up/in box
    sign_up_in_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(sign_up_in_box, "sign_up_in_box");
    gtk_widget_set_halign(sign_up_in_box, GTK_ALIGN_CENTER);

    // sign up button
    sign_up_button = gtk_button_new_with_label("SIGN UP");
    gtk_widget_set_name(sign_up_button, "sign_up_button");
    gtk_widget_set_size_request(sign_up_button, 400, 40);

    // sign in button
    sign_in_button = gtk_button_new_with_label("SIGN IN");
    gtk_widget_set_name(sign_in_button, "sign_in_button");
    gtk_widget_set_size_request(sign_in_button, 400, 40);

    // packing
    gtk_box_pack_start(GTK_BOX(sign_up_in_box), sign_up_button, FALSE, FALSE, 7);
    gtk_box_pack_start(GTK_BOX(sign_up_in_box), sign_in_button, FALSE, FALSE, 7);
    gtk_box_pack_start(GTK_BOX(logo_box), logo_image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(authorization_box), logo_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(authorization_box), sign_up_in_box, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(authorization_window), authorization_box);

    // signals
    g_signal_connect(sign_up_button, "clicked", G_CALLBACK(sign_up), main_struct);
    g_signal_connect(sign_in_button, "clicked", G_CALLBACK(sign_in), main_struct);
    g_signal_connect(authorization_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // show
    if (main_struct->sign_up_window) {
        gtk_widget_hide(main_struct->sign_up_window);
    }
    if (main_struct->sign_in_window) {
        gtk_widget_hide(main_struct->sign_in_window);
    }

    gtk_widget_show_all(authorization_window);

    // init struct
    main_struct->authorization_window = authorization_window;

    // plug
    if (!button) return;
}
