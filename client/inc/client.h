#ifndef CLIENT_H
#define CLIENT_H

#include <gtk/gtk.h>
#include <json-c/json.h>
#include <string.h>
#include <time.h>
#include "net.h"
#include <unistd.h>

typedef struct s_auth {
    guint id;
    gchar *login;
    gchar *username;
    gchar *password;
    gchar *password_repeat;
    gchar *image;
} t_auth;

typedef struct s_user {
    guint id;
    gchar *login;
    gchar *username;
    gchar *image;
} t_user;

typedef struct s_user_list {
    t_user *user;
    struct s_user_list *next;
} t_user_list;

typedef struct s_settings {
    GtkWidget *login_entry;
    GtkWidget *username_entry;
    GtkWidget *password_entry;
    GtkWidget *image_widget;
    gchar *image;
} t_settings;

typedef struct s_disconnect {
    gchar *disconnect_reconnecting_label;
    GtkWidget *disconnect_logo_image;
} t_disconnect;

typedef struct s_main_struct {
    char *ip;
    int port;

    gint argc;
    gchar **argv;
    gchar *theme;

    t_auth *auth;
    t_user_list *user_list;
    t_settings *settings;
    t_disconnect *disconnect;

    t_user *current;

    GtkCssProvider *provider;

    GtkWidget *uchat_window;
    GtkWidget *authorization_window;
    GtkWidget *sign_up_window;
    GtkWidget *sign_in_window;

    GtkWidget *login_is_empty;
    GtkWidget *login_is_too_short;
    GtkWidget *login_is_not_unique;

    GtkWidget *username_is_empty;
    GtkWidget *username_is_too_short;

    GtkWidget *password_is_empty;
    GtkWidget *password_is_too_short;

    GtkWidget *password_repeat_is_not_identical;

    GtkWidget *auth_is_failed;

    GtkWidget *settings_box;
    GtkWidget *uchat_box;
    GtkWidget *disconnect_box;

    GtkWidget *sidebar_box;
    GtkWidget *mainbar_box;
    GtkWidget *stickerbar_box;

    GtkWidget *header_user_image;
    GtkWidget *header_user_username_label;
    GtkWidget *header_user_login_label;
    GtkWidget *header_user_status;

    GtkWidget *sidebar_self_chat_image;
    GtkWidget *sidebar_self_chat_username_label;
    GtkWidget *sidebar_self_chat_login_label;

    GtkWidget *sidebar_currnet_chat_button;
    GtkWidget *sidebar_currnet_chat_image;
    GtkWidget *sidebar_currnet_chat_username_label;
    GtkWidget *sidebar_currnet_chat_login_label;
    GtkWidget *sidebar_currnet_chat_status;

    GtkWidget *sidebar_scrolled_chats;
    GtkWidget *mainbar_scrolled_chat;
    GtkWidget *stickerbar_scrolled;

    GtkWidget *sidebar_scrolled_chats_box;
    GtkWidget *mainbar_scrolled_chat_box;
    GtkWidget *stickerbar_scrolled_grid;

    GtkWidget *mainbar_input_entry;

    GtkWidget *search_box;
    GtkWidget *search_box_search_entry;

    // ~~~~~ for rerender ~~~~~ //

    GtkWidget *header_logo_image;

    GtkWidget *panel_settings_image;
    GtkWidget *panel_search_image;
    GtkWidget *panel_shop_image;
    GtkWidget *panel_exit_image;

    GtkWidget *mainbar_input_box_clip_image;
    GtkWidget *mainbar_input_box_emoji_image;
    GtkWidget *mainbar_input_box_send_image;

    GtkWidget *search_box_back_image;
    GtkWidget *search_box_add_image;

    GtkWidget *profile_password_show_image;
    GtkWidget *profile_image_set_image;
} t_main_struct;

// Global
extern t_main_struct *main_struct;

// Init
t_auth *init_auth();
gchar *init_user_image(gchar *login);
t_main_struct *init_main_struct(int argc, char *argv[]);

// Authorization
void authorization(GtkWidget *button, t_main_struct *main_struct);

// Sign up
void sign_up(GtkWidget *button, t_main_struct *main_struct);
void sign_up_update_login(GtkWidget *entry, t_main_struct *main_struct);
void sign_up_update_username(GtkWidget *entry, t_main_struct *main_struct);
void sign_up_update_password(GtkWidget *entry, t_main_struct *main_struct);
void sign_up_update_password_repeat(GtkWidget *entry, t_main_struct *main_struct);
void sign_up_submit(GtkWidget *button, t_main_struct *main_struct);

// Sign in
void sign_in(GtkWidget *button, t_main_struct *main_struct);
void sign_in_update_login(GtkWidget *entry, t_main_struct *main_struct);
void sign_in_update_password(GtkWidget *entry, t_main_struct *main_struct);
void sign_in_submit(GtkWidget *button, t_main_struct *main_struct);
void sign_in_success_auth();

// Users
t_user *user_new(guint id, gchar *login, gchar *username);
t_user_list *user_list_new(guint id, gchar *login, gchar *username);
void user_list_push(t_user_list **user_list, guint id, gchar *login, gchar *username);
guint user_list_size(t_user_list *user_list);
void user_list_parse_from_json(t_user_list **user_list, const gchar *str);
void user_list_parse_from_json_array(t_user_list **user_list, const gchar *str);
gchar *user_list_get_user_login_by_id(t_user_list *user_list, guint id);

// Uchat
void uchat(GtkWidget *button, t_main_struct *main_struct);
void uchat_sidebar_chat_new(t_main_struct *main_struct, t_user *current);
void uchat_sidebar_chat_self_new(t_main_struct *main_struct);
void uchat_mainbar_chat_new(t_main_struct *main_struct, t_user *current);
void uchat_mainbar_chat_scroll(t_main_struct *main_struct);
void *uchat_mainbar_chat_scroll_thread();
void uchat_stickerbar_sticker_new(t_main_struct *main_struct, gint column, gint row);
void uchat_disconnect_open(t_main_struct *main_struct);
void uchat_disconnect_close(t_main_struct *main_struct);
void uchat_settings_open(GtkWidget *button, t_main_struct *main_struct);
void uchat_settings_close(GtkWidget *button, t_main_struct *main_struct);
void uchat_settings_close_with_save(GtkWidget *button, t_main_struct *main_struct);
void uchat_settings_set_light_theme(GtkWidget *button, t_main_struct *main_struct);
void uchat_settings_set_default_theme(GtkWidget *button, t_main_struct *main_struct);
void uchat_settings_set_dark_theme(GtkWidget *button, t_main_struct *main_struct);
void uchat_settings_set_theme_provider(t_main_struct *main_struct);
void uchat_settings_set_username(GtkWidget *entry);
void uchat_settings_set_password(GtkWidget *entry);
void uchat_settings_set_image(GtkWidget *button, t_main_struct *main_struct);
void uchat_settings_toggle_password_visibility(GtkWidget *button, t_main_struct *main_struct);
void uchat_search_user_open(GtkWidget *button, t_main_struct *main_struct);
void uchat_search_user_close(GtkWidget *button, t_main_struct *main_struct);
void uchat_search_user_add(GtkWidget *button, t_main_struct *main_struct);
void uchat_seach_user_add_network();
void uchat_show_current_chat(t_main_struct *main_struct);
void uchat_switch_current_chat(GtkWidget *button, t_main_struct *main_struct);
void uchat_switch_mainbar_size(GtkWidget *button, t_main_struct *main_struct);
void uchat_send_text_message(GtkWidget *button, t_main_struct *main_struct);
void uchat_send_sticker_message(GtkWidget *button, t_main_struct *main_struct);
void uchat_send_file_message(GtkWidget *button, t_main_struct *main_struct);
void uchat_recieve_text_message(guint id, gchar *message, gboolean sound);
void uchat_recieve_sticker_message(guint id, gchar *sticker, gboolean sound);
void uchat_recieve_file_message(guint id, gchar *path, gboolean sound);
void uchat_load_text_message(gint id, gchar *message);
void uchat_load_sticker_message(gint id, gchar *sticker);
void uchat_load_file_message(gint id, gchar *filename);

// Jamconfig
gchar *jamconfig_parse_json(t_main_struct *main_struct);
void jamconfig_update_theme(gchar *theme);

// Utils
gchar *itoa(gint number);
void strdel(gchar **str);
gchar *strjoin(const gchar *s1, const gchar *s2);

#endif
