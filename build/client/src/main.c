#include "../inc/header.h"
static t_client client = {0};

enum State {UNAUTH, AUTH}

//char *construct_message(int uid, const char* text) {

//}

static void onConnect(dyad_Event *e) {
    printf("connected: %s\n", e->msg);
    dyad_writef(e->remote, "Hi\n");
}

static void onData(dyad_Event *e) {
    printf("%s", e->data); //TODO Remove debug
    if (client.state == UNAUTH) { // Authorization process
        client.state = AUTH;
        client.uid = strtoul(e->data);
    } else if (client.state == AUTH) {
        dyad_writef(e->remote, "Hi\n");
    }
}

int main() {
    char server_name[SERVER_NAME_LEN_MAX + 1] = "127.0.0.1";
    int port = 8000;
    dyad_init();
    
    dyad_Stream *s = dyad_newStream();
    dyad_addListener(s, DYAD_EVENT_CONNECT, onConnect, NULL);
    dyad_addListener(s, DYAD_EVENT_DATA,    onData,    NULL);
    dyad_connect(s, server_name, port);
    //TODO Check connection
    
    
    
    while (dyad_getStreamCount() > 0) {
        dyad_update();
    }
    
    dyad_shutdown();
    return 0;
}
