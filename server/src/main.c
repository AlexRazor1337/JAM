#include "../inc/header.h"
sqlite3 *db;
t_list *connections;

static void db_prepare() {
    db_exec(db, "CREATE TABLE IF NOT EXISTS users (id integer PRIMARY KEY AUTOINCREMENT, name varchar, login varchar UNIQUE, password varchar, last_visit datetime);", NULL);
    db_exec(db, "CREATE TABLE IF NOT EXISTS messages (id integer PRIMARY KEY AUTOINCREMENT, id_sender integer, id_reciever integer, text text, timestamp datetime, attachment integer, type integer);", NULL);
}

static void register_user(char *name, char *login, char *password) {
    char *str = malloc(196);
    sprintf(str, "INSERT INTO users(name, login, password) VALUES('%s', '%s', '%s');", name, login, password);
    db_exec(db, str, NULL);
    strdel(&str);
}

char *constructMsgJson(int sender_id, char *text, unsigned int timestamp, int type) {
    (void) timestamp;
    // TODO Finish this
    char *result = NULL;
    result = malloc(sizeof(int) * 3 + strlen(text) + 128);
    sprintf(result, "{\"sender\":\"%d\",\"type\":\"%d\",\"data\":\"%s\"}", sender_id, type, text);

    return result;
}

static void loadMsgHistory(int id) {
    char **result_table = NULL;
    int rows, cols;
    char *querry = malloc(96 + sizeof(int));
    sprintf(querry, "SELECT id_sender, text, timestamp, type FROM messages WHERE id_reciever = '%d' or id_sender = '%d';", id, id);
    sqlite3_get_table(db, querry, &result_table, &rows, &cols, NULL);
    if (rows > 0) {
        vec_str_t v;
        vec_init(&v);
        for (int i = 0; i < rows; i++){
            char *user_json = constructMsgJson(atoi(result_table[(i + 1) * cols]), result_table[(i + 1) * cols + 1], atol(result_table[(i + 1) * cols + 2]), atoi(result_table[(i + 1) * cols + 3]));
            vec_push(&v, user_json);
            //strdel(&user_json);
        }
    //TODO CLEAR VEC
        if (v.length > 1) {
            char *json = jsonlist_from_jsones(v, (sizeof(int) * 4 + strlen(result_table[1 * cols + 1]) + 128) * v.length);
            t_connection *client = find_node_uid(id, connections);
            if (client) dyad_writef(client->stream, "/@loadmsg|%b", json, strlen(json));
            strdel(&json);
            vec_deinit(&v);
        } else {
            t_connection *client = find_node_uid(id, connections);
            if (client) dyad_writef(client->stream, "/@loadmsg|[%b]", v.data[0], strlen(v.data[0]));
        }
        vec_deinit(&v);
    }

    sqlite3_free_table(result_table);
}

static void handleMsg(char *msg) {
    int *id = malloc(sizeof(int));
    char *reciever_id = malloc(sizeof(int));
    int *type = malloc(sizeof(int));
    char *data = malloc(strlen(msg));

    sscanf(msg, "/@%d/msg|%[^|]|%d|%[^\r]", id, reciever_id, type, data);
    if (id && reciever_id && data) {

        char *querry = malloc(strlen(msg) + 256);
        sprintf(querry, "INSERT INTO messages(id_sender, id_reciever, text, timestamp, type) VALUES('%d', '%s', '%s', '%ld', '%d');", *id, reciever_id, data, time(NULL), *type);
        db_exec(db, querry, NULL);
        strdel(&querry);

        t_connection *reciever = find_node_uid(atoi(reciever_id), connections);
        char *json = constructMsgJson(*id, data, time(NULL), *type);
        if (reciever) dyad_writef(reciever->stream, "/@msg|%b", json, strlen(json));
        strdel(&json);
    }
    free(id);
    id = NULL;
    free(type);
    type = NULL;
    strdel(&reciever_id);
    strdel(&data);
}


static void handleGetDialogsList(int id) {
    char **result_table = NULL;
    int rows, cols;
    char *querry = malloc(238 + sizeof(int) * 2);
    sprintf(querry, "SELECT id, name, login, last_visit FROM users WHERE id != '%d' AND id IN (SELECT DISTINCT id_sender FROM messages WHERE id_reciever = '%d' UNION SELECT DISTINCT id_reciever FROM messages WHERE id_sender = '%d');", id, id, id);
    sqlite3_get_table(db, querry, &result_table, &rows, &cols, NULL);
    if (rows > 0) {
        vec_str_t v;
        vec_init(&v);
        for (int i = 0; i < rows; i++) {
            char *user_json = malloc(38 + LOGIN_SIZE + USERNAME_SIZE + sizeof(int));
            sprintf(user_json, "{\"id\":\"%s\",\"name\":\"%s\",\"login\":\"%s\"}\n", result_table[(i + 1) * cols], result_table[(i + 1) * cols + 1], result_table[(i + 1) * cols + 2]);
            vec_push(&v, user_json);
            strdel(&user_json);
        }

        if (v.length > 1) {
            char *json = jsonlist_from_jsones(v, (38 + LOGIN_SIZE + USERNAME_SIZE + sizeof(int)) * v.length);
            t_connection *client = find_node_uid(id, connections);
            if (client) dyad_writef(client->stream, "/@updmsg|%b", json, strlen(json));
            strdel(&json);
        } else {
            t_connection *client = find_node_uid(id, connections);
            if (client) dyad_writef(client->stream, "/@updmsg|[%b]", v.data[0], strlen(v.data[0]));
        }
        vec_deinit(&v);
    } else {
        t_connection *client = find_node_uid(id, connections);
        if (client) dyad_writef(client->stream, "/@updmsg|[]");
    }

    sqlite3_free_table(result_table);
    strdel(&querry);
    dyad_update();
    loadMsgHistory(id);
}


void reverseAddUser(int asker_id, char *id) {
    char **result_table = NULL;
    int rows, cols;
    char *querry = malloc(66 + LOGIN_SIZE);
    sprintf(querry, "SELECT id, name, login, last_visit FROM users WHERE id = '%d';", asker_id);
    sqlite3_get_table(db, querry, &result_table, &rows, &cols, NULL);
    if (rows > 0) {
        char *user_json = malloc(256);
        sprintf(user_json, "{\"id\":\"%s\",\"name\":\"%s\",\"login\":\"%s\"}\n", result_table[1 * cols + 0], result_table[1 * cols + 1], result_table[1 * cols + 2]);

        t_connection *client = find_node_uid(atoi(id), connections);
        if (client) dyad_writef(client->stream, "/@adduser|%b", user_json, strlen(user_json));
        strdel(&user_json);
    } else {
        t_connection *client = find_node_uid(atoi(id), connections);
        if (client) dyad_writef(client->stream, "/@adduser|{}");
    }

    sqlite3_free_table(result_table);
}

static void handleAddUser(char *data) {
    int *id = malloc(sizeof(int));
    char *login = malloc(LOGIN_SIZE);
    sscanf(data, "/@%d/%*[^|]|%s", id, login);

    char **result_table = NULL;
    int rows, cols;
    char *querry = malloc(66 + LOGIN_SIZE);
    sprintf(querry, "SELECT id, name, login, last_visit FROM users WHERE login = '%s';", login);
    sqlite3_get_table(db, querry, &result_table, &rows, &cols, NULL);
    if (rows > 0) {
        char *user_json = malloc(256);
        sprintf(user_json, "{\"id\":\"%s\",\"name\":\"%s\",\"login\":\"%s\"}\n", result_table[1 * cols + 0], result_table[1 * cols + 1], result_table[1 * cols + 2]);
        reverseAddUser(*id, result_table[1 * cols]);

        t_connection *client = find_node_uid(*id, connections);
        if (client) dyad_writef(client->stream, "/@adduser|%b", user_json, strlen(user_json));
        strdel(&user_json);
    } else {
        t_connection *client = find_node_uid(*id, connections);
        if (client) dyad_writef(client->stream, "/@adduser|{}");
    }

    sqlite3_free_table(result_table);
    strdel(&login);
    free(id);
    id = NULL;
}

static void postAuthData(dyad_Event *e) {
    printf("PAD: %s\n", e->data);
    char *action = malloc(16);
    int *id = malloc(sizeof(int));
    sscanf(e->data, "/@%d/%[^|]|", id, action);

    if (strcmp(action, "msg") == 0) handleMsg(e->data);
    else if (strcmp(action, "getchats") == 0) handleGetDialogsList(*id);
    else if (strcmp(action, "adduser") == 0) handleAddUser(e->data);
    strdel(&action);
    free(id);
    id = NULL;
}

static void getAuthDetails(dyad_Event *e) {
    printf("%s| %d\n", e->data, mx_list_size(connections));

    char *operation = malloc(16);
    char *username = malloc(USERNAME_SIZE);
    int *id = malloc(sizeof(int));
    char *login = malloc(LOGIN_SIZE);
    char *password = malloc(PASSWORD_SIZE);
    char *querry = malloc(196);
    char *existing_user = NULL;

    sscanf(e->data, "/@%*d/%[^|]|", operation);
    if (strncmp(operation, "signup", 6) == 0) {
        sscanf(e->data, "/@%d/signup|%[^|]|%[^|]|%s", id, login, password, username);

        char *check_querry = malloc(196);
        sprintf(check_querry, "SELECT id FROM users WHERE login = '%s' LIMIT 1;", login);
        db_exec(db, check_querry, &existing_user);
        if (!existing_user) register_user(username, login, password);
        strdel(&check_querry);
    } else sscanf(e->data, "/@%d/authorize|%[^|]|%s", id, login, password);

    char **result_table = NULL;
    int rows = 0, cols  = 0;
    sprintf(querry, "SELECT id, name FROM users WHERE login = '%s' AND password = '%s' LIMIT 1;", login, password); // TODO update visit
    sqlite3_get_table(db, querry, &result_table, &rows, &cols, NULL);

    bool already_logged = false;
    if (rows > 0) {
        t_list *carret = connections;
        while (carret) {
            if (((t_connection *)carret->data)->uid == atoi(result_table[(0 + 1) * cols + 0])) {
                already_logged = true;
                break;
            }
            carret = carret->next;
        }

    }

    t_connection *client = find_node(*id, connections);
    if (!already_logged && client && (!existing_user) && rows > 0) {
        client->uid = atoi(result_table[(0 + 1) * cols + 0]);
        printf("Client Authorized\n");
        dyad_removeListener(client->stream, DYAD_EVENT_DATA, getAuthDetails, NULL);

        dyad_writef(client->stream, "/@/auth_answer|%s|%s", result_table[(0 + 1) * cols + 0], result_table[(0 + 1) * cols + 1]);  // Sends uid to client
        dyad_addListener(client->stream, DYAD_EVENT_DATA, postAuthData, NULL);
    } else if (client && !result_table) {
        dyad_writef(client->stream, "/@/auth_answer|-1");
    } else if (client) {
        dyad_writef(client->stream, "/@/auth_answer|-1");
    }

    sqlite3_free_table(result_table);
    strdel(&username);
    strdel(&login);
    strdel(&password);
    strdel(&querry);
    free(id);
    id = NULL;
}


static void firstConnectionEvent(dyad_Event *e) {
    t_connection *connection = create_connection(e->remote, connections);
    mx_push_back(&connections, connection);
    dyad_addListener(e->remote, DYAD_EVENT_DATA, getAuthDetails, NULL);
    dyad_writef(e->remote, "%d", connection->id);
}

static void check_disconnected_client() {
    t_list *carret = connections;
    int index = 0;
    while (carret) {
        t_connection *con = carret->data;
        if (dyad_getState(con->stream) == DYAD_STATE_CLOSED) {
            free(con);
            con = NULL;
            mx_pop_index(&connections, index);
        }
        index++;
        carret = carret->next;
    }
}


int main(int argc, char *argv[]) {
    //TODO uncomment
    //daemonize();
    if (argc < 2) {
        write(STDERR_FILENO, "usage: ./uchat_server port\n", 28);
        exit(EXIT_FAILURE);
    }

#if defined(__linux__)
    setvbuf(stdout, NULL, _IONBF, 0);
#endif
    char *result;

    printf("INITIALIZING JAM SERVER 🤡\n");
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
    dyad_listen(serv, atoi(argv[1]));

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

#pragma endregion sockets_init
//TODO Delete
    // register_user("MemoMmm", "memo", "qwerty");
    // register_user("Ayyyyy", "lmao", "qwerty");
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
    printf("\nBye-bye!\n");
    dyad_shutdown();
    exit(EXIT_SUCCESS);
}

//TODO 2. Add history loading
//TODO 3. Add files handling
