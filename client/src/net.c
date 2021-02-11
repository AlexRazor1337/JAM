#include "client.h"
t_client client = {0};
dyad_Stream *server_stream;
t_connect_data *connect_data;

void send_message(char *id, char *text) {
    printf("text: %s\n", text);
    dyad_writef(server_stream, "/@%d/msg|%s|%b", client.uid, id, text, strlen(text));
}

void onDataPostAuth(dyad_Event *e) {  // Anything, when user is AUTH'ed
    printf("Post Auth: %s\n", e->data);
    if (strncmp("/@updmsg", e->data, 8) == 0) {
        client.json_data = malloc(strlen(e->data));
        sscanf(e->data, "/@updmsg|%[^\r]", client.json_data);
    }
}

void addUser(char *login) {
    dyad_writef(server_stream, "/@%d/adduser|%s", client.uid, login);
}

void onDataWaitAuthAnswer(dyad_Event *e) {  // get real user id
    g_print("Real ID: %s\n", e->data);
    char *id = malloc(16);
    // TODO read username
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
        g_print("Failed auth!\n");
    }
}

void onConnectedData(dyad_Event *e) {  // Gets temporal user id
    printf("Temporal ID: %s\n", e->data);
    int id = atoi(e->data);
    printf("ASK FOR AUTH\n");

    dyad_removeListener(server_stream, DYAD_EVENT_DATA, onConnectedData, NULL);
    dyad_addListener(server_stream, DYAD_EVENT_DATA, onDataWaitAuthAnswer, NULL);
    //TODO
    if (connect_data->to_sign_up) {
        dyad_writef(server_stream, "/@%d/signup|%s|%s|%s", id, client.login, client.password, client.username);
    } else {
        dyad_writef(server_stream, "/@%d/authorize|%s|%s", id, client.login, client.password);
    }
}

static void onInitialConnect(dyad_Event *e) { printf("%s\n", e->msg); }

void onDisconnect() {
    client.state = UNAUTH;
    sleep(3);
    connectToServer();
}

void connectToServer() {
    server_stream = dyad_newStream();
    dyad_addListener(server_stream, DYAD_EVENT_CONNECT, onInitialConnect, NULL);
    dyad_addListener(server_stream, DYAD_EVENT_DATA, onConnectedData, NULL);
    dyad_connect(server_stream, SERVER_ADRESS, SERVER_PORT);  //TODO Check connection
}

void *serverInit(void *argument) {
    connect_data = (t_connect_data *)argument;

    connectToServer();
    client.state = UNAUTH;

    while (dyad_getStreamCount() > 0) {  // main loop
        if (client.state == AUTH) {
            if (dyad_getState(server_stream) == DYAD_STATE_CLOSED) onDisconnect();
        }
        if (client.state == AUTH_FAILED) {
            pthread_exit(&connect_data->thread);
        }
        dyad_update();
    }

    //    dyad_shutdown();
    return NULL;
}
