#include "../inc/header.h"
sqlite3 *db;
t_list *connections;

static void db_prepare() {
    db_exec(db, "CREATE TABLE IF NOT EXISTS users (id integer PRIMARY KEY AUTOINCREMENT, name varchar, login varchar UNIQUE, password varchar, last_visit datetime);", NULL);
    db_exec(db, "CREATE TABLE IF NOT EXISTS messages (id integer PRIMARY KEY AUTOINCREMENT, id_sender integer, id_reciever integer, text text, timestamp datetime, attachment integer);", NULL);
}

static void register_user(char *name, char *login, char *password) {
    char *str = malloc(196);
    sprintf(str, "INSERT INTO users(name, login, password) VALUES('%s', '%s', '%s');", name, login, password);
    db_exec(db, str, NULL);
    strdel(&str);
}


// char *constructMsgJson(char *chat_id, char *sender_id, char *text, unsigned int timestamp) {
//     (void)timestamp;
//     // TODO Finish this
//     char *result = malloc(strlen(chat_id) + strlen(sender_id) + strlen(text) + 128);
//     //sprintf("{}")
//     return result;
// }


static void handleMsg(char *data) {
    int *id = malloc(sizeof(int));
    char *reciever_id = malloc(sizeof(int));
    char *text = malloc(strlen(data));
    sscanf(data, "/@%d/msg|%[^|]|%[^\r]", id, reciever_id, text);
    if (id && reciever_id && text) {
        char *querry = malloc(strlen(data) + 256);  // TODO make it bigger depending on message size
        sprintf(querry, "INSERT INTO messages(id_sender, id_reciever, text, timestamp) VALUES('%d', '%s', '%s', '%ld');", *id, reciever_id, text, time(NULL));
        db_exec(db, querry, NULL);
        strdel(&querry);
        t_connection *reciever = find_node_uid(atoi(reciever_id), connections);
        if (reciever) dyad_writef(reciever->stream, "/@%d/msg|%b", find_node_uid(*id, connections)->uid, text, strlen(text));
    }

    free(id);
    id = NULL;
    strdel(&reciever_id);
    strdel(&text);
}


static void handleChatsUpdate(int id) {
    char **result_table = NULL;
    int rows, cols;
    char *querry = malloc(238 + sizeof(int) * 2);
    sprintf(querry, "SELECT id, name, login, last_visit FROM users WHERE id != '%d' AND id IN (SELECT DISTINCT id_sender FROM messages WHERE id_reciever = '%d' UNION SELECT DISTINCT id_reciever FROM messages WHERE id_sender = '%d');", id, id, id);
    sqlite3_get_table(db, querry, &result_table, &rows, &cols, NULL);
    if (rows > 0) {
        t_list *users_info = NULL;
        for (int i = 0; i < rows; i++) {
            char *user_json = malloc(38 + LOGIN_SIZE + USERNAME_SIZE + sizeof(int));
            sprintf(user_json, "{\"id\":\"%s\",\"name\":\"%s\",\"login\":\"%s\"}\n", result_table[(i + 1) * cols], result_table[(i + 1) * cols + 1], result_table[(i + 1) * cols + 2]);
            mx_push_back(&users_info, strdup(user_json));
            strdel(&user_json);
        }

        char *json = jsonlist_from_jsones(users_info, (38 + LOGIN_SIZE + USERNAME_SIZE + sizeof(int)) * mx_list_size(users_info));
        t_connection *client = find_node_uid(id, connections);
        if (client) dyad_writef(client->stream, "/@updmsg|%b", json, strlen(json));
        strdel(&json);
    } else {
        t_connection *client = find_node_uid(id, connections);
        if (client) dyad_writef(client->stream, "/@updmsg|[]");
    }

    sqlite3_free_table(result_table);
    strdel(&querry);
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
    else if (strcmp(action, "getchats") == 0) handleChatsUpdate(*id);
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

    //    sscanf("{\"temporal\":\"1\",\"login\":\"hello\",\"password\":\"pass\"}", "{\"temporal\":\"%d\",\"login\":\"%[^\"]\",\"password\":\"%[^\"]\'}", id, login, password);
    char **result_table = NULL;
    int rows = 0, cols  = 0;
    sprintf(querry, "SELECT id, name FROM users WHERE login = '%s' AND password = '%s' LIMIT 1;", login, password); // TODO update visit
    sqlite3_get_table(db, querry, &result_table, &rows, &cols, NULL);

    t_connection *client = find_node(*id, connections);
    if (client && (!existing_user) && rows > 0) {
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


int main() {
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
    dyad_listen(serv, 8000);

    signal(SIGINT, signal_handler);
#pragma endregion sockets_init
    register_user("MemoMmm", "memo", "qwerty");
    register_user("Ayyyyy", "lmao", "qwerty");
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

//TODO List
// 1. Add user
// 2. Stickers
// 3. Files
