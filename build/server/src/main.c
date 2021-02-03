#include "../inc/header.h"
sqlite3 *db;
t_list *connections;

void db_prepare() {
    db_exec(db, "CREATE TABLE IF NOT EXISTS users (id integer PRIMARY KEY AUTOINCREMENT, name varchar, login varchar UNIQUE, password varchar, last_visit datetime);", NULL);
    db_exec(db, "CREATE TABLE IF NOT EXISTS messages (id_from integer, id_to integer, text text, timestamp datetime, attachment integer);", NULL);
    db_exec(db, "CREATE TABLE IF NOT EXISTS files (id integer PRIMARY KEY AUTOINCREMENT, hash varchar, name varchar, extension varchar);", NULL);
}

void register_user(char *name, char *login, char *password) {
    char *str = malloc(196);
    sprintf(str, "INSERT INTO users(name, login, password) VALUES('%s', '%s', '%s');", name, login, password);
    db_exec(db, str, NULL);
    free(str);
}

void postAuthData(dyad_Event *e) {
    printf("PAD: %s\n", e->data);
    int *id = malloc(sizeof(int));
    char *reciever_id = malloc(16);
    char *text = malloc(196);
    sscanf(e->data, "/@%d/msg|%[^|]|%s", id, reciever_id, text);

    t_connection *reciever = find_node_uid(atoi(reciever_id), connections);

    dyad_writef(reciever->stream, "/@%d/msg|%s", find_node_uid(*id, connections)->uid, text);
    free(id);
    free(reciever_id);
    free(text);
}

void getAuthDetails(dyad_Event *e) {
    printf("%s\n", e->data);
    char* result;

    int *id = malloc(sizeof(int));
    char *login = malloc(64);
    char *password = malloc(64);
    char *str = malloc(196);

    sscanf(e->data, "/@%d/authorize|%[^|]|%s", id, login, password);
    sprintf(str, "SELECT id FROM users WHERE login = '%s' AND password = '%s' LIMIT 1;", login, password);
    db_exec(db, str, &result);

    t_connection *client = find_node(*id, connections);
    if (client) {
        client->uid = atoi(result);
        printf("Client Authorized:\n");
        dyad_removeListener(client->stream, DYAD_EVENT_DATA, getAuthDetails, NULL);
        dyad_addListener(client->stream, DYAD_EVENT_DATA, postAuthData, NULL);
        dyad_writef(client->stream, "/@/auth_answer|%s", result); // Sends uid to client
    }
    free(id);
    free(login);
    free(password);
    free(str);
}


//TODO Write create_connection()
static void firstConnectionEvent(dyad_Event *e) {
    t_connection *new_connection = malloc(sizeof(t_connection));
    new_connection->dataListener = getAuthDetails;
    new_connection->stream = e->remote;
    new_connection->id = generate_unique_id(connections);
    mx_push_back(&connections, new_connection);
    dyad_addListener(e->remote, DYAD_EVENT_DATA, getAuthDetails, NULL);
    dyad_writef(e->remote, "%d", new_connection->id);
}


int main() {
    #if defined(__linux__)
        setvbuf(stdout, NULL, _IONBF, 0);
    #endif
    char* result;

    printf("INITIALIZING JAM SERVER\n");
    printf("INITIALIZING SQLITE: ");
    #pragma region db_init
    if (!is_dir_exists("server_data"))
        mkdir("server_data", 0755);

    int rc = sqlite3_open("server_data/main.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    db_prepare();
    #pragma endregion db_init

    db_exec(db, "SELECT SQLITE_VERSION()", &result);
    printf("VER. %s\n", result);

    #pragma region sockets_init
    dyad_init();
    printf("INITIALIZING DYAD: VER. %s\n", dyad_getVersion());

    dyad_Stream *serv = dyad_newStream();
    dyad_addListener(serv, DYAD_EVENT_ACCEPT, firstConnectionEvent, NULL);
    dyad_listen(serv, 8000);

    signal(SIGINT, signal_handler);
    #pragma endregion sockets_init

    // TODO Remove when DB is consistent
    register_user("Pomogite", "mem", "testpas");
    register_user("Lel", "mem2", "testpas");
    while (dyad_getStreamCount() > 0) {
        dyad_update();
    }

    dyad_shutdown();
    sqlite3_close(db);
    return 0;
}

void signal_handler(int signal_number) {
    (void) signal_number;
    /* TODO: Put exit cleanup code here. */
    printf("Bye-bye!\n");
    dyad_shutdown();
    exit(EXIT_SUCCESS);
}
