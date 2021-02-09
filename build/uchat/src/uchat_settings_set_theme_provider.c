#include "client.h"

void uchat_settings_set_theme_provider(t_main_struct *main_struct) {
    gchar *theme = NULL;

    theme = strjoin("resource/styles/", main_struct->theme);
    theme = strjoin(theme, ".css");

    g_object_unref(main_struct->provider);

    main_struct->provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(main_struct->provider, theme, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(main_struct->provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    strdel(&theme);
}
