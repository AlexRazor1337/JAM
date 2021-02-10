#include "client.h"

t_main_struct *init_main_struct(int argc, char *argv[]) {
    t_main_struct *main_struct = (t_main_struct *)malloc(sizeof(t_main_struct));

    main_struct->argc = argc;
    main_struct->argv = argv;

    main_struct->auth = init_auth();

    main_struct->uchat_window = NULL;
    main_struct->authorization_window = NULL;
    main_struct->sign_up_window = NULL;
    main_struct->sign_in_window = NULL;

    gchar *theme = jamconfig_parse_json(main_struct);

    main_struct->provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(main_struct->provider, theme, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(main_struct->provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    strdel(&theme);

    return main_struct;
}
