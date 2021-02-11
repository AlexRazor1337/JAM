#include "client.h"

void user_list_parse_from_json(t_user_list **user_list, const gchar *str) {
    json_object *json = json_tokener_parse(str);
    array_list *array = json_object_get_array(json);

    for (size_t index = 0; index < array->length; index++) {
        json_object *data = (json_object *) array->array[index];
        gchar *username = NULL;
        gchar *login = NULL;
        gint id;

        json_object_object_foreach(data, key, value) {
            if (!strcmp(key, "id")) {
                id = (gint) json_object_get_int64(value);
            } else if (!strcmp(key, "login")) {
                login = strdup((gchar *) json_object_get_string(value));
            } else if (!strcmp(key, "name")) {
                username = strdup((gchar *) json_object_get_string(value));
            }
        }

        user_list_push(user_list, id, login, username); // id
    
        strdel(&login);
        strdel(&username);
    }


}
