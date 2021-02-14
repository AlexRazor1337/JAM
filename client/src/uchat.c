#include "client.h"

void uchat(GtkWidget *button, t_main_struct *main_struct) {
    // init
    GtkWidget *uchat_window;
    GtkWidget *main_fixed;

    GtkWidget *settings_box;
    GtkWidget *disconnect_box;
    GtkWidget *uchat_box;

    // window
    uchat_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(uchat_window), "JAM");
    gtk_window_set_resizable(GTK_WINDOW(uchat_window), FALSE);
    gtk_window_set_position(GTK_WINDOW(uchat_window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(uchat_window), 1400, 900);

    // main fixed
    main_fixed = gtk_fixed_new();
    gtk_widget_set_name(main_fixed, "main_fixed");

    // main uchat fixed
    uchat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(uchat_box, "uchat_box");
    gtk_widget_set_size_request(uchat_box, 1400, 900);

    // uchat...

    // init inner uchat fixed
    GtkWidget *header_box;
    GtkWidget *content_box;

    header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(header_box, "header_box");
    gtk_widget_set_size_request(header_box, 1380, 75);

    GtkWidget *header_logo_box;
    GtkWidget *header_logo_image;

    header_logo_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(header_logo_box, "header_logo_box");
    gtk_widget_set_size_request(header_logo_box, 300, 75);
    gtk_widget_set_halign(header_logo_box, GTK_ALIGN_START);

    header_logo_image = gtk_image_new();
    if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_file(GTK_IMAGE(header_logo_image), "resource/images/default/logo.png");
    } else if (!strcmp(main_struct->theme, "light")) {
        gtk_image_set_from_file(GTK_IMAGE(header_logo_image), "resource/images/light/logo.png");
    } else if (!strcmp(main_struct->theme, "dark")) {
        gtk_image_set_from_file(GTK_IMAGE(header_logo_image), "resource/images/dark/logo.png");
    }
    gtk_widget_set_name(header_logo_image, "header_logo_image");
    gtk_widget_set_size_request(header_logo_image, 180, 75);
    gtk_widget_set_halign(header_logo_image, GTK_ALIGN_START);

    GtkWidget *header_user_fixed;
    GtkWidget *header_user_image;
    GtkWidget *header_user_username_label;
    GtkWidget *header_user_login_label;
    GtkWidget *header_user_status;

    header_user_fixed = gtk_fixed_new();
    gtk_widget_set_name(header_user_fixed, "header_user_fixed");
    gtk_widget_set_size_request(header_user_fixed, 1080, 75);
    gtk_widget_set_halign(header_user_fixed, GTK_ALIGN_START);

    header_user_image = gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_file_at_scale(main_struct->auth->image, 50, 50, FALSE, NULL));  // dinamic cgange
    gtk_widget_set_name(header_user_image, "header_user_image");
    gtk_widget_set_size_request(header_user_image, 100, 75);
    gtk_widget_set_halign(header_user_image, GTK_ALIGN_CENTER);

    header_user_username_label = gtk_label_new(main_struct->auth->username);  // dinamic cgange
    gtk_widget_set_name(header_user_username_label, "header_user_username_label");

    header_user_login_label = gtk_label_new(main_struct->auth->login);  // dinamic cgange
    gtk_widget_set_name(header_user_login_label, "header_user_login_label");

    header_user_status = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(header_user_status, "online");
    gtk_widget_set_size_request(header_user_status, 10, 10);

    main_struct->header_user_image = header_user_image;
    main_struct->header_user_username_label = header_user_username_label;
    main_struct->header_user_login_label = header_user_login_label;
    main_struct->header_user_status = header_user_status;

    content_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(content_box, "content_box");
    gtk_widget_set_size_request(content_box, 1380, 795);

    GtkWidget *sidebar_box;
    GtkWidget *mainbar_box;
    GtkWidget *stickerbar_box;

    sidebar_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(sidebar_box, "sidebar_box");
    gtk_widget_set_size_request(sidebar_box, 300, 795);

    mainbar_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(mainbar_box, "mainbar_box");
    gtk_widget_set_size_request(mainbar_box, 780, 795);

    stickerbar_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(stickerbar_box, "stickerbar_box");
    gtk_widget_set_size_request(stickerbar_box, 300, 795);

    main_struct->sidebar_box = sidebar_box;
    main_struct->mainbar_box = mainbar_box;
    main_struct->stickerbar_box = stickerbar_box;

    GtkWidget *sidebar_chat_box;
    GtkWidget *sidebar_panel_box;

    sidebar_chat_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(sidebar_chat_box, "sidebar_chat_box");
    gtk_widget_set_size_request(sidebar_chat_box, 300, 720);

    sidebar_panel_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(sidebar_panel_box, "sidebar_panel_box");
    gtk_widget_set_size_request(sidebar_panel_box, 300, 75);

    GtkWidget *sidebar_scrolled_chats;
    GtkWidget *sidebar_scrolled_chats_box;

    sidebar_scrolled_chats = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_name(sidebar_scrolled_chats, "sidebar_scrolled_chats");
    gtk_widget_set_size_request(sidebar_scrolled_chats, 280, 690);
    gtk_scrolled_window_set_kinetic_scrolling(GTK_SCROLLED_WINDOW(sidebar_scrolled_chats), TRUE);

    sidebar_scrolled_chats_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(sidebar_scrolled_chats_box, "sidebar_scrolled_chats_box");
    gtk_widget_set_size_request(sidebar_scrolled_chats, 280, 690);
    main_struct->sidebar_scrolled_chats_box = sidebar_scrolled_chats_box;

    // create users
    uchat_sidebar_chat_self_new(main_struct);

    t_user_list *user_list_iter_cpy = main_struct->user_list;
    user_list_iter_cpy = user_list_iter_cpy->next;

    // call from search of from parsed users_list
    while (user_list_iter_cpy) {
        uchat_sidebar_chat_new(main_struct, user_list_iter_cpy->user);

        user_list_iter_cpy = user_list_iter_cpy->next;
    }

    GtkWidget *panel_settings_button;
    GtkWidget *panel_search_button;
    GtkWidget *panel_shop_button;
    GtkWidget *panel_exit_button;
    GtkWidget *panel_settings_image;
    GtkWidget *panel_search_image;
    GtkWidget *panel_shop_image;
    GtkWidget *panel_exit_image;

    panel_settings_button = gtk_button_new();
    gtk_widget_set_name(panel_settings_button, "panel_settings_button");
    gtk_widget_set_size_request(panel_settings_button, 75, 75);

    panel_settings_image = gtk_image_new();
    if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_file(GTK_IMAGE(panel_settings_image), "resource/images/default/settings.png");
    } else if (!strcmp(main_struct->theme, "light")) {
        gtk_image_set_from_file(GTK_IMAGE(panel_settings_image), "resource/images/light/settings.png");
    } else if (!strcmp(main_struct->theme, "dark")) {
        gtk_image_set_from_file(GTK_IMAGE(panel_settings_image), "resource/images/dark/settings.png");
    }
    gtk_widget_set_name(panel_settings_image, "panel_settings_image");

    panel_search_button = gtk_button_new();
    gtk_widget_set_name(panel_search_button, "panel_search_button");
    gtk_widget_set_size_request(panel_search_button, 75, 75);

    panel_search_image = gtk_image_new();
    if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_file(GTK_IMAGE(panel_search_image), "resource/images/default/search.png");
    } else if (!strcmp(main_struct->theme, "light")) {
        gtk_image_set_from_file(GTK_IMAGE(panel_search_image), "resource/images/light/search.png");
    } else if (!strcmp(main_struct->theme, "dark")) {
        gtk_image_set_from_file(GTK_IMAGE(panel_search_image), "resource/images/dark/search.png");
    }
    gtk_widget_set_name(panel_search_image, "panel_search_image");

    panel_shop_button = gtk_button_new();
    gtk_widget_set_name(panel_shop_button, "panel_shop_button");
    gtk_widget_set_size_request(panel_shop_button, 75, 75);

    panel_shop_image = gtk_image_new();
    if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_file(GTK_IMAGE(panel_shop_image), "resource/images/default/shop.png");
    } else if (!strcmp(main_struct->theme, "light")) {
        gtk_image_set_from_file(GTK_IMAGE(panel_shop_image), "resource/images/light/shop.png");
    } else if (!strcmp(main_struct->theme, "dark")) {
        gtk_image_set_from_file(GTK_IMAGE(panel_shop_image), "resource/images/dark/shop.png");
    }
    gtk_widget_set_name(panel_shop_image, "panel_shop_image");

    panel_exit_button = gtk_button_new();
    gtk_widget_set_name(panel_exit_button, "panel_exit_button");
    gtk_widget_set_size_request(panel_exit_button, 75, 75);

    panel_exit_image = gtk_image_new();
    if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_file(GTK_IMAGE(panel_exit_image), "resource/images/default/exit.png");
    } else if (!strcmp(main_struct->theme, "light")) {
        gtk_image_set_from_file(GTK_IMAGE(panel_exit_image), "resource/images/light/exit.png");
    } else if (!strcmp(main_struct->theme, "dark")) {
        gtk_image_set_from_file(GTK_IMAGE(panel_exit_image), "resource/images/dark/exit.png");
    }
    gtk_widget_set_name(panel_exit_image, "panel_exit_image");

    GtkWidget *mainbar_chat_box;
    GtkWidget *mainbar_input_box;

    mainbar_chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(mainbar_chat_box, "mainbar_chat_box");
    gtk_widget_set_size_request(mainbar_chat_box, 760, 720);

    mainbar_input_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(mainbar_input_box, "mainbar_input_box");
    gtk_widget_set_size_request(mainbar_input_box, 760, 75);

    GtkWidget *mainbar_scrolled_chat;
    GtkWidget *mainbar_scrolled_chat_box;

    mainbar_scrolled_chat = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_name(mainbar_scrolled_chat, "mainbar_scrolled_chat");
    gtk_widget_set_size_request(mainbar_scrolled_chat, 760, 710);
    gtk_scrolled_window_set_kinetic_scrolling(GTK_SCROLLED_WINDOW(mainbar_scrolled_chat), TRUE);

    mainbar_scrolled_chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(mainbar_scrolled_chat_box, "mainbar_scrolled_chat_box");
    gtk_widget_set_size_request(mainbar_scrolled_chat_box, 740, 690);
    main_struct->mainbar_scrolled_chat_box = mainbar_scrolled_chat_box;

    user_list_iter_cpy = main_struct->user_list;

    while (user_list_iter_cpy) {
        uchat_mainbar_chat_new(main_struct, user_list_iter_cpy->user);

        user_list_iter_cpy = user_list_iter_cpy->next;
    }

    GtkWidget *mainbar_input_box_clip_button;
    GtkWidget *mainbar_input_box_clip_image;
    GtkWidget *mainbar_input_box_input_entry;
    GtkWidget *mainbar_input_box_emoji_button;
    GtkWidget *mainbar_input_box_emoji_image;
    GtkWidget *mainbar_input_box_send_button;
    GtkWidget *mainbar_input_box_send_image;

    mainbar_input_box_clip_button = gtk_button_new();
    gtk_widget_set_name(mainbar_input_box_clip_button, "mainbar_input_box_clip_button");
    gtk_widget_set_size_request(mainbar_input_box_clip_button, 75, 75);

    mainbar_input_box_clip_image = gtk_image_new();
    if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_file(GTK_IMAGE(mainbar_input_box_clip_image), "resource/images/default/clip.png");
    } else if (!strcmp(main_struct->theme, "light")) {
        gtk_image_set_from_file(GTK_IMAGE(mainbar_input_box_clip_image), "resource/images/light/clip.png");
    } else if (!strcmp(main_struct->theme, "dark")) {
        gtk_image_set_from_file(GTK_IMAGE(mainbar_input_box_clip_image), "resource/images/dark/clip.png");
    }
    gtk_widget_set_name(mainbar_input_box_clip_image, "mainbar_input_box_clip_image");

    mainbar_input_box_input_entry = gtk_entry_new();
    gtk_widget_set_name(mainbar_input_box_input_entry, "mainbar_input_box_input_entry");
    gtk_widget_set_size_request(mainbar_input_box_input_entry, 535, 75);
    gtk_entry_set_placeholder_text(GTK_ENTRY(mainbar_input_box_input_entry), "Write your message...");
    main_struct->mainbar_input_entry = mainbar_input_box_input_entry;

    mainbar_input_box_emoji_button = gtk_button_new();
    gtk_widget_set_name(mainbar_input_box_emoji_button, "mainbar_input_box_emoji_button");
    gtk_widget_set_size_request(mainbar_input_box_emoji_button, 75, 75);

    mainbar_input_box_emoji_image = gtk_image_new();
    if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_file(GTK_IMAGE(mainbar_input_box_emoji_image), "resource/images/default/emoji.png");
    } else if (!strcmp(main_struct->theme, "light")) {
        gtk_image_set_from_file(GTK_IMAGE(mainbar_input_box_emoji_image), "resource/images/light/emoji.png");
    } else if (!strcmp(main_struct->theme, "dark")) {
        gtk_image_set_from_file(GTK_IMAGE(mainbar_input_box_emoji_image), "resource/images/dark/emoji.png");
    }
    gtk_widget_set_name(mainbar_input_box_emoji_image, "mainbar_input_box_emoji_image");

    mainbar_input_box_send_button = gtk_button_new();
    gtk_widget_set_name(mainbar_input_box_send_button, "mainbar_input_box_send_button");
    gtk_widget_set_size_request(mainbar_input_box_send_button, 75, 75);

    mainbar_input_box_send_image = gtk_image_new();
    if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_file(GTK_IMAGE(mainbar_input_box_send_image), "resource/images/default/send.png");
    } else if (!strcmp(main_struct->theme, "light")) {
        gtk_image_set_from_file(GTK_IMAGE(mainbar_input_box_send_image), "resource/images/light/send.png");
    } else if (!strcmp(main_struct->theme, "dark")) {
        gtk_image_set_from_file(GTK_IMAGE(mainbar_input_box_send_image), "resource/images/dark/send.png");
    }
    gtk_widget_set_name(mainbar_input_box_send_image, "mainbar_input_box_send_image");

    GtkWidget *stickerbar_scrolled;
    GtkWidget *stickerbar_scrolled_grid;

    stickerbar_scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_name(stickerbar_scrolled, "stickerbar_scrolled");
    gtk_widget_set_size_request(stickerbar_scrolled, 300, 795);
    gtk_scrolled_window_set_kinetic_scrolling(GTK_SCROLLED_WINDOW(stickerbar_scrolled), TRUE);

    stickerbar_scrolled_grid = gtk_grid_new();
    gtk_widget_set_name(stickerbar_scrolled_grid, "stickerbar_scrolled_grid");
    gtk_grid_set_row_spacing(GTK_GRID(stickerbar_scrolled_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(stickerbar_scrolled_grid), 5);
    main_struct->stickerbar_scrolled_grid = stickerbar_scrolled_grid;

    for (gint column = 0; column < 5; column++) {
        for (gint row = 0; row < 30; row++) {
            uchat_stickerbar_sticker_new(main_struct, column, row);
        }
    }

    GtkWidget *search_box;
    GtkWidget *search_box_back_button;
    GtkWidget *search_box_search_entry;
    GtkWidget *search_box_search_button;
    GtkWidget *search_box_back_image;
    GtkWidget *search_box_add_image;

    search_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(search_box, "search_box");
    gtk_widget_set_size_request(search_box, 300, 75);

    search_box_back_button = gtk_button_new();
    gtk_widget_set_name(search_box_back_button, "search_box_back_button");
    gtk_widget_set_size_request(search_box_back_button, 75, 75);

    search_box_back_image = gtk_image_new();
    if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(search_box_back_image), gdk_pixbuf_new_from_file_at_scale("resource/images/default/close.png", 32, 32, TRUE, NULL));
    } else if (!strcmp(main_struct->theme, "light")) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(search_box_back_image), gdk_pixbuf_new_from_file_at_scale("resource/images/light/close.png", 32, 32, TRUE, NULL));
    } else if (!strcmp(main_struct->theme, "dark")) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(search_box_back_image), gdk_pixbuf_new_from_file_at_scale("resource/images/dark/close.png", 32, 32, TRUE, NULL));
    }
    gtk_widget_set_name(search_box_back_image, "search_box_back_image");
    gtk_widget_set_halign(search_box_back_image, GTK_ALIGN_CENTER);

    search_box_search_entry = gtk_entry_new();
    gtk_widget_set_name(search_box_search_entry, "search_box_search_entry");
    gtk_widget_set_size_request(search_box_search_entry, 150, 75);

    search_box_search_button = gtk_button_new();
    gtk_widget_set_name(search_box_search_button, "search_box_search_button");
    gtk_widget_set_size_request(search_box_search_button, 75, 75);

    search_box_add_image = gtk_image_new();
    if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(search_box_add_image), gdk_pixbuf_new_from_file_at_scale("resource/images/default/add.png", 32, 32, TRUE, NULL));
    } else if (!strcmp(main_struct->theme, "light")) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(search_box_add_image), gdk_pixbuf_new_from_file_at_scale("resource/images/light/add.png", 32, 32, TRUE, NULL));
    } else if (!strcmp(main_struct->theme, "dark")) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(search_box_add_image), gdk_pixbuf_new_from_file_at_scale("resource/images/dark/add.png", 32, 32, TRUE, NULL));
    }
    gtk_widget_set_name(search_box_add_image, "search_box_add_image");
    gtk_widget_set_halign(search_box_add_image, GTK_ALIGN_CENTER);



    // Disconnect box
    disconnect_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 70);
    gtk_widget_set_name(disconnect_box, "disconnect_box");
    gtk_widget_set_size_request(disconnect_box, 1400, 900);
    gtk_widget_set_valign(disconnect_box, GTK_ALIGN_CENTER);



    // Settings

    GtkWidget *settings_main_box;
    GtkWidget *settings_main_profile_box;
    GtkWidget *settings_main_theme_box;
    GtkWidget *settings_main_control_box;

    // main settings fixed
    settings_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(settings_box, "settings_box");
    gtk_widget_set_size_request(settings_box, 1400, 900);

    settings_main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(settings_main_box, "settings_main_box");
    gtk_widget_set_halign(settings_main_box, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(settings_main_box, 760, 700);

    settings_main_profile_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(settings_main_profile_box, "settings_main_profile_box");
    gtk_widget_set_size_request(settings_main_profile_box, 730, 350);

    settings_main_theme_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(settings_main_theme_box, "settings_main_theme_box");
    gtk_widget_set_size_request(settings_main_theme_box, 730, 250);

    settings_main_control_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(settings_main_control_box, "settings_main_control_box");
    gtk_widget_set_size_request(settings_main_control_box, 730, 70);

    GtkWidget *profile_labels_fixed;
    GtkWidget *profile_entries_fixed;
    GtkWidget *profile_image_fixed;

    profile_labels_fixed = gtk_fixed_new();
    gtk_widget_set_name(profile_labels_fixed, "profile_labels_fixed");
    gtk_widget_set_size_request(profile_labels_fixed, 230, 310);

    profile_entries_fixed = gtk_fixed_new();
    gtk_widget_set_name(profile_entries_fixed, "profile_entries_fixed");
    gtk_widget_set_size_request(profile_entries_fixed, 230, 310);

    profile_image_fixed = gtk_fixed_new();
    gtk_widget_set_name(profile_image_fixed, "profile_image_fixed");
    gtk_widget_set_size_request(profile_image_fixed, 230, 310);

    GtkWidget *profile_profile_label;
    GtkWidget *profile_login_label;
    GtkWidget *profile_username_label;
    GtkWidget *profile_password_label;

    profile_profile_label = gtk_label_new("PROFILE");
    gtk_widget_set_name(profile_profile_label, "profile_profile_label");

    profile_login_label = gtk_label_new("LOGIN");
    gtk_widget_set_name(profile_login_label, "profile_login_label");

    profile_username_label = gtk_label_new("USERNAME");
    gtk_widget_set_name(profile_username_label, "profile_username_label");

    profile_password_label = gtk_label_new("PASSWORD");
    gtk_widget_set_name(profile_password_label, "profile_password_label");

    GtkWidget *profile_login_entry;
    GtkWidget *profile_username_entry;
    GtkWidget *profile_password_entry;
    GtkWidget *profile_password_show_button;
    GtkWidget *profile_password_show_image;

    profile_login_entry = gtk_entry_new();
    gtk_widget_set_name(profile_login_entry, "profile_login_entry");
    gtk_entry_set_text(GTK_ENTRY(profile_login_entry), main_struct->auth->login);
    gtk_entry_set_max_length(GTK_ENTRY(profile_login_entry), 16);
    gtk_entry_set_placeholder_text(GTK_ENTRY(profile_login_entry), "Your login...");
    gtk_widget_set_can_focus(profile_login_entry, FALSE);

    profile_username_entry = gtk_entry_new();
    gtk_widget_set_name(profile_username_entry, "profile_username_entry");
    gtk_entry_set_text(GTK_ENTRY(profile_username_entry), main_struct->auth->username);
    gtk_entry_set_max_length(GTK_ENTRY(profile_username_entry), 16);
    gtk_entry_set_placeholder_text(GTK_ENTRY(profile_username_entry), "Your username...");

    profile_password_entry = gtk_entry_new();
    gtk_widget_set_name(profile_password_entry, "profile_password_entry");
    gtk_entry_set_text(GTK_ENTRY(profile_password_entry), main_struct->auth->password);
    gtk_entry_set_max_length(GTK_ENTRY(profile_password_entry), 16);
    gtk_entry_set_placeholder_text(GTK_ENTRY(profile_password_entry), "Your pasword...");
    gtk_entry_set_visibility(GTK_ENTRY(profile_password_entry), FALSE);

    profile_password_show_button = gtk_button_new();
    gtk_widget_set_name(profile_password_show_button, "profile_password_show_button");

    profile_password_show_image = gtk_image_new();
    if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/default/eye.png", 20, 20, TRUE, NULL));
    } else if (!strcmp(main_struct->theme, "light")) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/light/eye.png", 20, 20, TRUE, NULL));
    } else if (!strcmp(main_struct->theme, "dark")) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(profile_password_show_image), gdk_pixbuf_new_from_file_at_scale("resource/images/dark/eye.png", 20, 20, TRUE, NULL));
    }
    gtk_widget_set_name(profile_password_show_image, "profile_password_show_image");

    GtkWidget *profile_image_button;
    GtkWidget *profile_image_inner_fixed;
    GtkWidget *profile_image_image;
    GtkWidget *profile_image_set_image;

    profile_image_button = gtk_button_new();
    gtk_widget_set_name(profile_image_button, "profile_image_button");
    gtk_widget_set_size_request(profile_image_button, 125, 125);

    profile_image_inner_fixed = gtk_fixed_new();
    gtk_widget_set_name(profile_image_inner_fixed, "profile_image_inner_fixed");
    gtk_widget_set_size_request(profile_image_inner_fixed, 125, 125);

    profile_image_image = gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_file_at_scale(main_struct->auth->image, 125, 125, FALSE, NULL));
    gtk_widget_set_name(profile_image_image, "profile_image_image");

    profile_image_set_image = gtk_image_new();

    if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_file(GTK_IMAGE(profile_image_set_image), "resource/images/default/plus.png");
    } else if (!strcmp(main_struct->theme, "light")) {
        gtk_image_set_from_file(GTK_IMAGE(profile_image_set_image), "resource/images/light/plus.png");
    } else if (!strcmp(main_struct->theme, "dark")) {
        gtk_image_set_from_file(GTK_IMAGE(profile_image_set_image), "resource/images/dark/plus.png");
    }
    
    gtk_widget_set_name(profile_image_set_image, "profile_image_set_image");
    gtk_widget_set_size_request(profile_image_set_image, 40, 40);
    gtk_widget_set_halign(profile_image_set_image, GTK_ALIGN_CENTER);



    // Disconnect

    GtkWidget *disconnect_reconnecting_label;
    GtkWidget *disconnect_logo_image;

    disconnect_reconnecting_label = gtk_label_new("RECONNECTING...");
    gtk_widget_set_name(disconnect_reconnecting_label, "disconnect_reconnecting_label");
    
    disconnect_logo_image = gtk_image_new();
    // if (!strcmp(main_struct->theme, "default")) {
        gtk_image_set_from_file(GTK_IMAGE(disconnect_logo_image), "resource/images/jam.png");
    // }
    // else if (!strcmp(main_struct->theme, "light")) {
    //     gtk_image_set_from_file(GTK_IMAGE(profile_image_set_image), "resource/images/light/plus.png");
    // }
    // else if (!strcmp(main_struct->theme, "dark")) {
    //     gtk_image_set_from_file(GTK_IMAGE(profile_image_set_image), "resource/images/dark/plus.png");
    // }
    gtk_widget_set_name(disconnect_logo_image, "disconnect_logo_image");
    // gtk_widget_set_size_request(disconnect_logo_image, 40, 40);
    // gtk_widget_set_halign(disconnect_logo_image, GTK_ALIGN_CENTER);

    // Themes

    GtkWidget *theme_light_box;
    GtkWidget *theme_default_box;
    GtkWidget *theme_dark_box;
    GtkWidget *theme_light_button;
    GtkWidget *theme_default_button;
    GtkWidget *theme_dark_button;

    theme_light_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(theme_light_box, "theme_light_box");
    gtk_widget_set_size_request(theme_light_box, 240, 210);

    theme_default_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(theme_default_box, "theme_default_box");
    gtk_widget_set_size_request(theme_default_box, 240, 210);

    theme_dark_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(theme_dark_box, "theme_dark_box");
    gtk_widget_set_size_request(theme_dark_box, 240, 210);

    theme_light_button = gtk_button_new_with_label("LIGHT");
    gtk_widget_set_name(theme_light_button, "theme_light_button");
    gtk_widget_set_size_request(theme_light_button, 150, 150);
    gtk_widget_set_halign(theme_light_button, GTK_ALIGN_CENTER);

    theme_default_button = gtk_button_new_with_label("DEFAUTL");
    gtk_widget_set_name(theme_default_button, "theme_default_button");
    gtk_widget_set_size_request(theme_default_button, 150, 150);
    gtk_widget_set_halign(theme_default_button, GTK_ALIGN_CENTER);

    theme_dark_button = gtk_button_new_with_label("DARK");
    gtk_widget_set_name(theme_dark_button, "theme_dark_button");
    gtk_widget_set_size_request(theme_dark_button, 150, 150);
    gtk_widget_set_halign(theme_dark_button, GTK_ALIGN_CENTER);

    GtkWidget *control_log_out_box;
    GtkWidget *control_cancel_box;
    GtkWidget *control_save_box;
    GtkWidget *control_log_out_button;
    GtkWidget *control_cancel_button;
    GtkWidget *control_save_button;

    control_log_out_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(control_log_out_box, "control_log_out_box");
    gtk_widget_set_size_request(control_log_out_box, 240, 60);

    control_cancel_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(control_cancel_box, "control_cancel_box");
    gtk_widget_set_size_request(control_cancel_box, 240, 60);

    control_save_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(control_save_box, "control_save_box");
    gtk_widget_set_size_request(control_save_box, 240, 60);

    control_log_out_button = gtk_button_new_with_label("LOG OUT");
    gtk_widget_set_name(control_log_out_button, "control_log_out_button");
    gtk_widget_set_size_request(control_log_out_button, 150, 50);
    gtk_widget_set_halign(control_log_out_button, GTK_ALIGN_CENTER);

    control_cancel_button = gtk_button_new_with_label("CANCEL");
    gtk_widget_set_name(control_cancel_button, "control_cancel_button");
    gtk_widget_set_size_request(control_cancel_button, 150, 50);
    gtk_widget_set_halign(control_cancel_button, GTK_ALIGN_CENTER);

    control_save_button = gtk_button_new_with_label("SAVE");
    gtk_widget_set_name(control_save_button, "control_save_button");
    gtk_widget_set_size_request(control_save_button, 150, 50);
    gtk_widget_set_halign(control_save_button, GTK_ALIGN_CENTER);

    // packing
    gtk_box_pack_start(GTK_BOX(header_logo_box), header_logo_image, FALSE, FALSE, 0);

    gtk_fixed_put(GTK_FIXED(header_user_fixed), header_user_image, 0, 0);
    gtk_fixed_put(GTK_FIXED(header_user_fixed), header_user_username_label, 85, 10);
    gtk_fixed_put(GTK_FIXED(header_user_fixed), header_user_login_label, 85, 38);
    gtk_fixed_put(GTK_FIXED(header_user_fixed), header_user_status, 70, 50);

    gtk_box_pack_start(GTK_BOX(header_box), header_logo_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(header_box), header_user_fixed, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(sidebar_scrolled_chats), sidebar_scrolled_chats_box);
    gtk_box_pack_start(GTK_BOX(sidebar_chat_box), sidebar_scrolled_chats, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(panel_settings_button), panel_settings_image);
    gtk_container_add(GTK_CONTAINER(panel_search_button), panel_search_image);
    gtk_container_add(GTK_CONTAINER(panel_shop_button), panel_shop_image);
    gtk_container_add(GTK_CONTAINER(panel_exit_button), panel_exit_image);

    gtk_box_pack_start(GTK_BOX(sidebar_panel_box), panel_settings_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_panel_box), panel_search_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_panel_box), panel_shop_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_panel_box), panel_exit_button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(sidebar_box), sidebar_chat_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar_box), sidebar_panel_box, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(mainbar_scrolled_chat), mainbar_scrolled_chat_box);
    gtk_box_pack_start(GTK_BOX(mainbar_chat_box), mainbar_scrolled_chat, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(mainbar_input_box_clip_button), mainbar_input_box_clip_image);
    gtk_container_add(GTK_CONTAINER(mainbar_input_box_emoji_button), mainbar_input_box_emoji_image);
    gtk_container_add(GTK_CONTAINER(mainbar_input_box_send_button), mainbar_input_box_send_image);

    gtk_box_pack_start(GTK_BOX(mainbar_input_box), mainbar_input_box_clip_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mainbar_input_box), mainbar_input_box_input_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mainbar_input_box), mainbar_input_box_emoji_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mainbar_input_box), mainbar_input_box_send_button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(mainbar_box), mainbar_chat_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mainbar_box), mainbar_input_box, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(stickerbar_scrolled), stickerbar_scrolled_grid);
    gtk_box_pack_start(GTK_BOX(stickerbar_box), stickerbar_scrolled, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(content_box), sidebar_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_box), mainbar_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(content_box), stickerbar_box, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(uchat_box), header_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(uchat_box), content_box, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(search_box_back_button), search_box_back_image);
    gtk_container_add(GTK_CONTAINER(search_box_search_button), search_box_add_image);

    gtk_box_pack_start(GTK_BOX(search_box), search_box_back_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(search_box), search_box_search_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(search_box), search_box_search_button, FALSE, FALSE, 0);

    gtk_fixed_put(GTK_FIXED(profile_labels_fixed), profile_profile_label, 50, 30);
    gtk_fixed_put(GTK_FIXED(profile_labels_fixed), profile_login_label, 50, 130);
    gtk_fixed_put(GTK_FIXED(profile_labels_fixed), profile_username_label, 50, 190);
    gtk_fixed_put(GTK_FIXED(profile_labels_fixed), profile_password_label, 50, 250);

    gtk_container_add(GTK_CONTAINER(profile_password_show_button), profile_password_show_image);

    gtk_fixed_put(GTK_FIXED(profile_entries_fixed), profile_login_entry, 50, 118);
    gtk_fixed_put(GTK_FIXED(profile_entries_fixed), profile_username_entry, 50, 178);
    gtk_fixed_put(GTK_FIXED(profile_entries_fixed), profile_password_entry, 50, 238);
    gtk_fixed_put(GTK_FIXED(profile_entries_fixed), profile_password_show_button, 180, 243);

    gtk_fixed_put(GTK_FIXED(profile_image_inner_fixed), profile_image_image, 0, 10);
    gtk_fixed_put(GTK_FIXED(profile_image_inner_fixed), profile_image_set_image, 110, 0);
    gtk_container_add(GTK_CONTAINER(profile_image_button), profile_image_inner_fixed);
    gtk_fixed_put(GTK_FIXED(profile_image_fixed), profile_image_button, 50, 25);

    gtk_box_pack_start(GTK_BOX(settings_main_profile_box), profile_labels_fixed, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(settings_main_profile_box), profile_entries_fixed, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(settings_main_profile_box), profile_image_fixed, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(theme_light_box), theme_light_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(theme_default_box), theme_default_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(theme_dark_box), theme_dark_button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(settings_main_theme_box), theme_light_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(settings_main_theme_box), theme_default_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(settings_main_theme_box), theme_dark_box, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(control_log_out_box), control_log_out_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(control_cancel_box), control_cancel_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(control_save_box), control_save_button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(settings_main_control_box), control_log_out_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(settings_main_control_box), control_cancel_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(settings_main_control_box), control_save_box, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(settings_main_box), settings_main_profile_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(settings_main_box), settings_main_theme_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(settings_main_box), settings_main_control_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(settings_box), settings_main_box, FALSE, FALSE, 0);


    // Disconnect
    gtk_box_pack_start(GTK_BOX(disconnect_box), disconnect_logo_image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(disconnect_box), disconnect_reconnecting_label, FALSE, FALSE, 0);

    gtk_fixed_put(GTK_FIXED(main_fixed), settings_box, 0, 0);
    gtk_fixed_put(GTK_FIXED(main_fixed), disconnect_box, 0, 0);
    gtk_fixed_put(GTK_FIXED(main_fixed), uchat_box, 0, 0);
    gtk_fixed_put(GTK_FIXED(main_fixed), search_box, 10, 815);

    gtk_container_add(GTK_CONTAINER(uchat_window), main_fixed);



    // signals
    g_signal_connect(panel_settings_button, "clicked", G_CALLBACK(uchat_settings_open), main_struct);
    g_signal_connect(panel_search_button, "clicked", G_CALLBACK(uchat_search_user_open), main_struct);
    g_signal_connect(panel_exit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(search_box_back_button, "clicked", G_CALLBACK(uchat_search_user_close), main_struct);
    g_signal_connect(search_box_search_entry, "activate", G_CALLBACK(uchat_search_user_add), main_struct);
    g_signal_connect(search_box_search_button, "clicked", G_CALLBACK(uchat_search_user_add), main_struct);

    g_signal_connect(mainbar_input_box_clip_button, "clicked", G_CALLBACK(uchat_send_file_message), main_struct);
    g_signal_connect(mainbar_input_box_input_entry, "activate", G_CALLBACK(uchat_send_text_message), main_struct);
    g_signal_connect(mainbar_input_box_send_button, "clicked", G_CALLBACK(uchat_send_text_message), main_struct);
    g_signal_connect(mainbar_input_box_emoji_button, "clicked", G_CALLBACK(uchat_switch_mainbar_size), main_struct);

    g_signal_connect(profile_username_entry, "changed", G_CALLBACK(uchat_settings_set_username), NULL);
    g_signal_connect(profile_password_entry, "changed", G_CALLBACK(uchat_settings_set_password), NULL);

    g_signal_connect(profile_password_show_button, "clicked", G_CALLBACK(uchat_settings_toggle_password_visibility), main_struct);
    g_signal_connect(profile_image_button, "clicked", G_CALLBACK(uchat_settings_set_image), main_struct);

    g_signal_connect(theme_light_button, "clicked", G_CALLBACK(uchat_settings_set_light_theme), main_struct);
    g_signal_connect(theme_default_button, "clicked", G_CALLBACK(uchat_settings_set_default_theme), main_struct);
    g_signal_connect(theme_dark_button, "clicked", G_CALLBACK(uchat_settings_set_dark_theme), main_struct);

    g_signal_connect(control_log_out_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(control_cancel_button, "clicked", G_CALLBACK(uchat_settings_close), main_struct);
    g_signal_connect(control_save_button, "clicked", G_CALLBACK(uchat_settings_close_with_save), main_struct);

    g_signal_connect(uchat_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // show
    if (main_struct->sign_up_window) {
        gtk_widget_hide(main_struct->sign_up_window);
    }
    if (main_struct->sign_in_window) {
        gtk_widget_hide(main_struct->sign_in_window);
    }

    gtk_widget_show(uchat_window);
    gtk_widget_show(main_fixed);

    gtk_widget_show_all(uchat_box);

    uchat_show_current_chat(main_struct);
    uchat_switch_mainbar_size(NULL, main_struct);

    gtk_widget_hide(gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(sidebar_scrolled_chats)));
    gtk_widget_hide(gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(mainbar_scrolled_chat)));
    gtk_widget_hide(gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(stickerbar_scrolled)));

    // init struct
    main_struct->uchat_window = uchat_window;

    main_struct->settings_box = settings_box;
    main_struct->disconnect_box = disconnect_box;
    main_struct->uchat_box = uchat_box;
    main_struct->search_box = search_box;

    main_struct->search_box_search_entry = search_box_search_entry;

    main_struct->sidebar_scrolled_chats = sidebar_scrolled_chats;
    main_struct->mainbar_scrolled_chat = mainbar_scrolled_chat;
    main_struct->stickerbar_scrolled = stickerbar_scrolled;

    // rerender for theme changing
    main_struct->header_logo_image = header_logo_image;

    main_struct->panel_settings_image = panel_settings_image;
    main_struct->panel_search_image = panel_search_image;
    main_struct->panel_shop_image = panel_shop_image;
    main_struct->panel_exit_image = panel_exit_image;

    main_struct->mainbar_input_box_clip_image = mainbar_input_box_clip_image;
    main_struct->mainbar_input_box_emoji_image = mainbar_input_box_emoji_image;
    main_struct->mainbar_input_box_send_image = mainbar_input_box_send_image;

    main_struct->search_box_back_image = search_box_back_image;
    main_struct->search_box_add_image = search_box_add_image;

    main_struct->profile_password_show_image = profile_password_show_image;
    main_struct->profile_image_set_image = profile_image_set_image;

    // settings
    main_struct->settings = (t_settings *)malloc(sizeof(t_settings));

    main_struct->settings->login_entry = profile_login_entry;
    main_struct->settings->username_entry = profile_username_entry;
    main_struct->settings->password_entry = profile_password_entry;
    main_struct->settings->image_widget = profile_image_image;
    main_struct->settings->image = strdup(main_struct->auth->image);

    // disconnect
    main_struct->disconnect = (t_disconnect *)malloc(sizeof(t_disconnect));

    // plug
    if (!button) return;
}
