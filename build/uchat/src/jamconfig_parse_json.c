#include "client.h"

gchar *jamconfig_parse_json(t_main_struct *main_struct) {
    json_object *json = json_object_from_file("resource/config/jamconfig.json");
    gchar *theme = strdup("resource/styles/");

    json_object_object_foreach(json, key, value) {
        if (!strcmp(key, "theme")) {
            theme = strjoin(theme, json_object_get_string(value));
            theme = strjoin(theme, ".css");

            main_struct->theme = strdup(json_object_get_string(value));
        }
    }

    return theme;
}
