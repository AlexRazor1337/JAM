#include "client.h"

t_client client = {0};
dyad_Stream *server_stream;
t_connect_data *connect_data;
bool reconnect = false;

char* replace_char(char* str, char find, char replace){
    char *current_pos = strchr(str,find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}

void sendMessage(size_t id, char *message, int type) {
    replace_char(message, '"', ' ');
    replace_char(message, '\\', ' ');
    dyad_writef(server_stream, "/@%d/msg|%d|%d|%b", client.uid, id, type, message, strlen(message));
}

// TODO: change msg -> filemsg (check right string formater) (uncomment in another file)
void sendSFileMessage(size_t id, char *filename, char *binary_content) {
    dyad_writef(server_stream, "/@%d/msg|%d|%b|%b", client.uid, id, filename, strlen(filename), binary_content, strlen(binary_content));
}

void onDataPostAuth(dyad_Event *e) {  // Anything, when user is AUTH'ed
    printf("Post Auth: %s\n", e->data);
    if (strncmp("/@updmsg", e->data, 8) == 0) {
        client.json_data = malloc(strlen(e->data));
        sscanf(e->data, "/@updmsg|%[^\r]", client.json_data);
    } else if (strncmp("/@adduser", e->data, 9) == 0) {
        client.json_data = malloc(strlen(e->data));
        sscanf(e->data, "/@adduser|%[^\r]", client.json_data);
        uchat_seach_user_add_network();
    } else if (strncmp("/@msg", e->data, 5) == 0) {
        char *data = malloc(strlen(e->data));
        sscanf(e->data, "/@msg|%[^\r]", data);

        json_object *json = json_tokener_parse(data);
        int type = json_object_get_int(json_object_object_get(json, "type"));

        json_object *sender_id = json_object_object_get(json, "sender");
        int id = json_object_get_int(sender_id);
        if (type == 0) {
            json_object *message_json = json_object_object_get(json, "data");
            char *message = (char *) json_object_get_string(message_json);

            uchat_recieve_text_message(id, message, TRUE);
        } else if (type == 1) {
            json_object *sticker_json = json_object_object_get(json, "data");
            char *sticker = (char *) json_object_get_string(sticker_json);

            uchat_recieve_sticker_message(id, sticker, TRUE);
        } else if (type == 2) {
            /**
             * JSON BE LIKE:
             * {
             *  "type": "file",
             *  "data": {
             *      "filename": "cat_gif_text.gif",
             *      "binary_content": "..."
             *  }
             * }
             */
            json_object *file_json = json_object_object_get(json, "data");
            char *filename = NULL;
            char *binary_content = NULL;

            json_object_object_foreach(file_json, key, value) {
                if (!strcmp(key, "filename")) {
                    filename = strdup(json_object_get_string(value));
                } else if (!strcmp(key, "binary_content")){
                    binary_content = strdup(json_object_get_string(value));
                }
            }

            char *path = strdup("resource/images/messages/");
            path = strjoin(path, filename);

            FILE *file = fopen(path, "wb");
            fputs(binary_content, file);
            fclose(file);

            uchat_recieve_file_message(id, filename, path);

            free(filename);
            filename = NULL;
            free(binary_content);
            binary_content = NULL;
            free(path);
            path = NULL;
        }
    } else if (strncmp("/@loadmsg", e->data, 9) == 0) {
        sleep(1);

        char *data = malloc(strlen(e->data));
        sscanf(e->data, "/@loadmsg|%[^\r]", data);

        json_object *json = json_tokener_parse(data);
        array_list *array = json_object_get_array(json);

        for (size_t index = 0; index < array->length; index++) {
            json_object *message_json = (json_object *) array->array[index];

            int type = (int) json_object_get_int(json_object_object_get(message_json, "type"));
            int sender = (int) json_object_get_int(json_object_object_get(message_json, "sender"));
            int reciever = (int) json_object_get_int(json_object_object_get(message_json, "id_reciever"));
            char *data= (char *) json_object_get_string(json_object_object_get(message_json, "data"));

            if (type == 0) {
                if (sender == (int) main_struct->auth->id) {
                    uchat_load_text_message(reciever, data);
                } else {
                    uchat_recieve_text_message(sender, data, FALSE);
                }
            } else if (type == 1) {
                if (sender == (int) main_struct->auth->id) {
                    uchat_load_sticker_message(reciever, data);
                } else {
                    uchat_recieve_sticker_message(sender, data, FALSE);
                }
            } else if (type == 2) {

            }
        }
    }
}

void addUser(char *login) {
    dyad_writef(server_stream, "/@%d/adduser|%s", client.uid, login);
}

void onDataWaitAuthAnswer(dyad_Event *e) {  // get real user id
    g_print("Real ID: %s\n", e->data);
    char *id = malloc(16);
    char *username = malloc(32);
    sscanf(e->data, "/@/auth_answer|%[^|]|%s", id, username);
    if (id && atoi(id) != -1) {
        client.uid = atoi(id);
        client.state = AUTH;
        free(id);
        client.username = strdup(username);
        free(username);
        dyad_removeListener(server_stream, DYAD_EVENT_DATA, onDataWaitAuthAnswer, NULL);
        dyad_addListener(server_stream, DYAD_EVENT_DATA, onDataPostAuth, NULL);
        dyad_writef(server_stream, "/@%d/getchats", client.uid);
    } else if (atoi(id) == -1) {
        client.state = AUTH_FAILED;
        dyad_close(server_stream);
        printf("Failed auth!\n");
    }
}

void onConnectedData(dyad_Event *e) {  // Gets temporal user id
    printf("Temporal ID: %s\n", e->data);
    int id = atoi(e->data);

    dyad_removeListener(server_stream, DYAD_EVENT_DATA, onConnectedData, NULL);
    dyad_addListener(server_stream, DYAD_EVENT_DATA, onDataWaitAuthAnswer, NULL);

    if (connect_data->to_sign_up) {
        dyad_writef(server_stream, "/@%d/signup|%s|%s|%s", id, client.login, client.password, client.username);
    } else {
        dyad_writef(server_stream, "/@%d/authorize|%s|%s", id, client.login, client.password);
    }
}

static void onInitialConnect(dyad_Event *e) {
    printf("%s\n", e->msg);

    if (reconnect) {
        uchat_disconnect_close(main_struct);
        reconnect = false;
    }
}

void onDisconnect() {
    client.state = UNAUTH;
    uchat_disconnect_open(main_struct);
    sleep(3);
    connectToServer();

    reconnect = true;
}

void connectToServer() {
    server_stream = dyad_newStream();
    dyad_addListener(server_stream, DYAD_EVENT_CONNECT, onInitialConnect, NULL);
    dyad_addListener(server_stream, DYAD_EVENT_DATA, onConnectedData, NULL);
    dyad_connect(server_stream, main_struct->ip, main_struct->port);
}

void *serverInit(void *argument) {
    connect_data = (t_connect_data *) argument;

    connectToServer();
    client.state = UNAUTH;
    
    while (true) {  // main loop
        if (client.state == AUTH) {
            if (dyad_getState(server_stream) == DYAD_STATE_CLOSED) onDisconnect();
        }
        if (client.state == AUTH_FAILED) {
            pthread_exit(&connect_data->thread);
        }
        dyad_update();
    }

    return NULL;
}
