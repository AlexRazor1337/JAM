#include "client.h"

void user_list_parse_from_json(t_user_list **user_list, const gchar *str) {
    json_object *json = json_tokener_parse(str);
    gchar *username = NULL;
    gchar *login = NULL;
    guint id;

    json_object_object_foreach(json, key, value) {
        if (!strcmp(key, "id")) {
            id = (gint) json_object_get_int64(value);
        } else if (!strcmp(key, "login")) {
            login = strdup((gchar *) json_object_get_string(value));
        } else if (!strcmp(key, "name")) {
            username = strdup((gchar *) json_object_get_string(value));
        }
    }

    if (id && login && username) {
        user_list_push(user_list, id, login, username);
    }

    strdel(&login);
    strdel(&username);
}
