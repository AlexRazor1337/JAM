#include "client.h"

void jamconfig_update_theme(gchar *theme) {
    json_object *json = json_object_from_file("resource/config/jamconfig.json");

    json_object_object_foreach(json, key, value) {
        if (!strcmp(key, "theme")) {
            json_object_set_string(value, theme);
        }
    }

    json_object_to_file("resource/config/jamconfig.json", json);
    json_object_put(json);
}
