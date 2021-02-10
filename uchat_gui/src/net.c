#include "client.h"
t_client client = {0};
dyad_Stream *server_stream;

void send_message(char *id, char *text) {
    printf("text: %s\n", text);
    dyad_writef(server_stream, "/@%d/msg|%s|%b", client.uid, id, text, strlen(text));
}

void onDataPostAuth(dyad_Event *e) {  // Anything, when user is AUTH'ed
    printf("Post Auth: %s\n", e->data);
}

void onDataWaitAuthAnswer(dyad_Event *e) {  // get real user id
    g_print("Real ID: %s\n", e->data);
    char *id = malloc(16);
    sscanf(e->data, "/@/auth_answer|%s", id);
    if (id && atoi(id) != -1) {
        client.uid = atoi(id);
        client.state = AUTH;
        free(id);

        dyad_removeListener(server_stream, DYAD_EVENT_DATA, onDataWaitAuthAnswer, NULL);
        dyad_addListener(server_stream, DYAD_EVENT_DATA, onDataPostAuth, NULL);
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

    dyad_writef(server_stream, "/@%d/authorize|%s|%s", id, client.login, client.password);
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
    t_connect_data *connect_data = (t_connect_data *)argument;

    connectToServer();
    //client.login = "mem";  //malloc(32);
    //client.password = "testpas";
    client.state = UNAUTH;

    // size_t clsize = 32;
    // getline(&client.login, &clsize, stdin);  //TODO remove thistemporal shit
    // sscanf(client.login, "%s\n", client.login);
    // printf("Your login is: %s", client.login);
    // TODO Temporal shit for the console
    // char *action = NULL;
    // char *id = malloc(32);
    // char *text = malloc(32);
    // size_t bufsize = 256;
    // char *buffer = malloc(256);

    while (dyad_getStreamCount() > 0) {  // main loop
        if (client.state == AUTH) {
            // action = malloc(32);
            // printf("Select action: '@write id text'");
            // getline(&buffer, &bufsize, stdin);
            // sscanf(buffer, "@%s %s %[^\f]", action, id, text);

            // if (action) {
            //     if (strncmp(action, "write", 5) == 0 && id && text) {
            //         send_message(id, text);
            //     }
            // }
            // free(action);  // TODO remove console shit
            // action = NULL;

            if (dyad_getState(server_stream) == DYAD_STATE_CLOSED) onDisconnect();
        }
        if (client.state == AUTH_FAILED) {
            pthread_exit(connect_data->thread);
        }
        dyad_update();
    }

    //    dyad_shutdown();
    return NULL;
}
