#include "../inc/header.h"
static t_client client = {0};
dyad_Stream *server_stream;

//TODO make struct
char *login;
char *password = "testpas";

static void onInitialConnect(dyad_Event *e) {
    printf("connected: %s\n", e->msg);
}

void onDataPostAuth(dyad_Event *e) {
    printf("Post Auth: %s\n", e->data);
    char *id = malloc(16);
    sscanf(e->data, "/@/auth_answer|%s", id);
    client.uid = atoi(id);
    client.state = AUTH;
    free(id);
    //TODO add new listener
}

void onConnectedData(dyad_Event *e) {
    int id = atoi(e->data);
    printf("%s\n", e->data);
    dyad_writef(server_stream, "/@%d/authorize|%s|%s", id, login, password);
    dyad_removeListener(server_stream, DYAD_EVENT_DATA, onConnectedData, NULL);
    dyad_addListener(server_stream, DYAD_EVENT_DATA, onDataPostAuth, NULL);
}

void send_message(char *id, char *text) {
    dyad_writef(server_stream, "/@%d/msg|%s|%s", client.uid, id, text);
}

int main() {
    char server_name[SERVER_NAME_LEN_MAX + 1] = "10.11.7.9";
    int port = 8000;
    dyad_init();
    login = malloc(32); //TODO Remove
    gets(login);
    server_stream = dyad_newStream();
    dyad_addListener(server_stream, DYAD_EVENT_CONNECT, onInitialConnect, NULL);
    dyad_addListener(server_stream, DYAD_EVENT_DATA, onConnectedData, NULL);
    dyad_connect(server_stream, server_name, port);
    //TODO Check connection
    client.state = UNAUTH;
    char *action = malloc(32);
    char *id = malloc(32);
    char *text = malloc(32);
    size_t bufsize = 256;
    char *buffer = malloc(256);
    while (dyad_getStreamCount() > 0) { //main loop
        if (client.state == AUTH) {
            printf("Select action: '@write id text'");
            getline(&buffer, &bufsize, stdin);
            sscanf(buffer, "@%s %s %s", action, id, text);
            if (action) {
                if (strncmp(action, "@write", 6) == 0 && id && text) {
                    send_message(id, text);
                }
                send_message(id, text);
            }
        }
        dyad_update();
    }

    dyad_shutdown();
    return 0;
}
