#include "../inc/header.h"
sqlite3 *db;
t_list *connections;

void db_prepare() {
    db_exec(db, "CREATE TABLE IF NOT EXISTS users (id integer PRIMARY KEY AUTOINCREMENT, name varchar, login varchar UNIQUE, password varchar, last_visit datetime);", NULL);
    db_exec(db, "CREATE TABLE IF NOT EXISTS messages (id integer PRIMARY KEY AUTOINCREMENT, id_from integer, chat_id integer, text text, timestamp datetime, attachment integer);", NULL);
    db_exec(db, "CREATE TABLE IF NOT EXISTS participants (id integer PRIMARY KEY AUTOINCREMENT, uid integer, chat_id integer);", NULL);
    db_exec(db, "CREATE TABLE IF NOT EXISTS chats (id integer PRIMARY KEY AUTOINCREMENT, name varchar, is_group BOOLEAN);", NULL);
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
    char *chat_id = malloc(16);
    char *text = malloc(196);
    sscanf(e->data, "/@%d/msg|%[^|]|%s", id, chat_id, text);
    if (id && chat_id && text) { // && atoi(chat_id) != 0
        //t_connection *reciever = find_node_uid(atoi(chat_id), connections);
        char *str = malloc(256);
        sprintf(str, "INSERT INTO messages(id_from, chat_id, text, timestamp) VALUES('%d', '%s', '%s', '%ld');", *id, chat_id, text, time(NULL));
        db_exec(db, str, NULL);
        free(str);
        char* result;
        db_exec(db, "SELECT id FROM participants WHERE chat_id='1'", &result);
        //TODO get users
        char** tmp = NULL;
        int num_rows, num_cols;
        char* zErrMsg = NULL;
        int rc = sqlite3_get_table(db, statement, &tmp, &num_rows, &num_cols, &zErrMsg);
        if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        exit(EXIT_FAILURE);
        }
        char** result = (char **)malloc(sizeof(char *)*(num_cols*num_rows));
        for(int i = 0; i < num_cols*num_rows; i++){
            //if(i%num_cols == 0) printf("%s\n", "");
            //printf("%s   ", result[i]);
            result[i] = strdup(tmp[i+num_cols]);
            //printf("%s   ", result[i]);
        }
        //printf("%s\n", "");
        sqlite3_free_table(tmp);
        //if (reciever) dyad_writef(reciever->stream, "/@%d/msg|%s", find_node_uid(*id, connections)->uid, text);
    }

    free(id);
    free(chat_id);
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

        dyad_writef(client->stream, "/@/auth_answer|%s", result); // Sends uid to client
        dyad_addListener(client->stream, DYAD_EVENT_DATA, postAuthData, NULL);
    }
    free(result);
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

void check_disconnected_client() {
    t_list *carret = connections;
    int index = 0;
    while (carret) {
        t_connection *con = carret->data;
        if (dyad_getState(con->stream) == DYAD_STATE_CLOSED) {
            free(con);
            con = NULL;
            mx_pop_index(&connections, index);
        }
        carret = carret->next;
    }
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
    char *str = malloc(196);
    sprintf(str, "INSERT INTO chats(is_group) VALUES('%d');", false);
    db_exec(db, str, NULL);
    free(str);
    str = NULL;

    db_exec(db,  "INSERT INTO participants(uid, chat_id) VALUES('1', '1');", NULL);
    db_exec(db,  "INSERT INTO participants(uid, chat_id) VALUES('2', '1');", NULL);


    while (dyad_getStreamCount() > 0) {
        dyad_update();
        check_disconnected_client();
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
