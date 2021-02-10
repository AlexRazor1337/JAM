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

static char *jsonlist_from_jsones(t_list *list, int bsize) {
    char *final_json = malloc(bsize);
    char *temporal = malloc(bsize);

    char *cursor = temporal;
    char *cursor_2 = final_json;
    if (mx_list_size(list) == 1) {
        sprintf(final_json, "[%s]", (char *)list->data);
        free(temporal);
        return final_json;
    }

    t_list *carret = list;
    sprintf(final_json, "[%s", (char *)carret->data);
    carret = carret->next;
    while (carret) {
        if (carret->next) {
            sprintf(cursor, "%s,%s", cursor_2, (char *)carret->data);
            free(cursor_2);
            cursor_2 = malloc(bsize);
            char *temp = cursor_2;
            cursor_2 = cursor;
            cursor = temp;
        } else {
            sprintf(cursor, "%s,%s]", cursor_2, (char *)carret->data);
            free(cursor_2);
            cursor_2 = NULL;
        }
        carret = carret->next;
    }

    return cursor;
}

char *constructMsgJson(char *chat_id, char *sender_id, char *text, unsigned int timestamp) {
    (void)timestamp;
    // TODO Finish this
    char *result = malloc(strlen(chat_id) + strlen(sender_id) + strlen(text) + 128);
    //sprintf("{}")
    return result;
}

void handleMsgUpdate() {
}

void handleMsg(char *data) {
    int *id = malloc(sizeof(int));
    char *chat_id = malloc(16);
    char *text = malloc(strlen(data));
    sscanf(data, "/@%d/msg|%[^|]|%[^\r]", id, chat_id, text);
    if (id && chat_id && text) {                 // && atoi(chat_id) != 0
        char *str = malloc(strlen(data) + 256);  // TODO make it bigger depending on message size
        sprintf(str, "INSERT INTO messages(id_from, chat_id, text, timestamp) VALUES('%d', '%s', '%s', '%ld');", *id, chat_id, text, time(NULL));
        db_exec(db, str, NULL);
        free(str);
        str = NULL;

        char **result_table = NULL;
        int num_rows, num_cols;
        char *querry = malloc(96);
        sprintf(querry, "SELECT id FROM participants WHERE chat_id='%s'", chat_id);
        sqlite3_get_table(db, querry, &result_table, &num_rows, &num_cols, NULL);
        char **result = (char **)malloc(sizeof(char *) * (num_cols * num_rows));
        for (int i = 0; i < num_cols * num_rows; i++) {
            result[i] = strdup(result_table[i + num_cols]);
            t_connection *reciever = find_node_uid(atoi(result[i]), connections);
            if (reciever) dyad_writef(reciever->stream, "/@%d/msg|%b", find_node_uid(*id, connections)->uid, text, strlen(text));
            free(result[i]);
        }

        sqlite3_free_table(result_table);
        free(querry);
        free(result);
    }

    free(id);
    free(chat_id);
    free(text);
}

void handleChatsUpdate(int id) {
    // get chat ids
    char **result_table = NULL;
    int num_rows, num_cols;
    char *querry = malloc(238 + 32);
    sprintf(querry, "SELECT id, name, login, last_visit FROM users WHERE id IN (SELECT uid FROM participants WHERE uid != '%d' AND chat_id IN (SELECT id FROM chats WHERE is_group = '0' AND id IN (SELECT DISTINCT chat_id FROM participants WHERE uid = '%d')));", id, id);
    sqlite3_get_table(db, querry, &result_table, &num_rows, &num_cols, NULL);

    t_list *users_info = NULL;
    for (int i = 0; i < num_rows; i++) {
        char *temp = malloc(256);
        sprintf(temp, "{\"id\":\"%s\",\"name\":\"%s\",\"login\":\"%s\"}\n", result_table[(i + 1) * num_cols + 0], result_table[(i + 1) * num_cols + 1], result_table[(i + 1) * num_cols + 2]);
        mx_push_back(&users_info, strdup(temp));
        free(temp);
    }

    char *json = jsonlist_from_jsones(users_info, 96 * mx_list_size(users_info));
    sqlite3_free_table(result_table);
    result_table = NULL;
    
    t_connection *client = find_node_uid(id, connections);
    if (client) dyad_writef(client->stream, "/updmsg|%b", json, strlen(json));
    
    free(querry);
    free(json);
    //get chat info from ids
    //GET USERS AND GROUPS SEPARATELY
    //(row + 1) * num_cols + col

    // TODO Clear strings inside chat_ids
    
}

void postAuthData(dyad_Event *e) {
    printf("PAD: %s\n", e->data);
    char *action = malloc(32);
    sscanf(e->data, "/@%*d/%[^|]|", action);

    if (strcmp(action, "msg") == 0) {
        handleMsg(e->data);
    } 
    // else if (strcmp(action, "getchats") == 0) {
    //     handleChatsUpdate(e->data);
    // }
    free(action);
}

void getAuthDetails(dyad_Event *e) {
    printf("%s| %d\n", e->data, mx_list_size(connections));

    char *operation = malloc(32);
    char *username = malloc(32);
    int *id = malloc(sizeof(int));
    char *login = malloc(64);
    char *password = malloc(64);
    char *str = malloc(196);
    char *check_result = NULL;

    sscanf(e->data, "/@%*d/%[^|]|", operation);
    
    if (strncmp(operation, "signup", 6) == 0) {
        sscanf(e->data, "/@%d/signup|%[^|]|%[^|]|%s", id, login, password, username);
        
        char *str_check = malloc(196);
        sprintf(str_check, "SELECT id FROM users WHERE login = '%s' LIMIT 1;", login);
        db_exec(db, str_check, &check_result);
        if (!check_result) register_user(username, login, password);

        free(str_check);
    } else {
        sscanf(e->data, "/@%d/authorize|%[^|]|%s", id, login, password);
    }
    
    //    sscanf("{\"temporal\":\"1\",\"login\":\"hello\",\"password\":\"pass\"}", "{\"temporal\":\"%d\",\"login\":\"%[^\"]\",\"password\":\"%[^\"]\'}", id, login, password);
    char **result_table = NULL;
    int num_rows = 0, num_cols  = 0;
    sprintf(str, "SELECT id, name FROM users WHERE login = '%s' AND password = '%s' LIMIT 1;", login, password);
    sqlite3_get_table(db, str, &result_table, &num_rows, &num_cols, NULL);

    t_connection *client = find_node(*id, connections);

    if (client && (!check_result) && num_rows > 0) {
        client->uid = atoi(result_table[(0 + 1) * num_cols + 0]);
        printf("Client Authorized:\n");
        dyad_removeListener(client->stream, DYAD_EVENT_DATA, getAuthDetails, NULL);

        dyad_writef(client->stream, "/@/auth_answer|%s|%s", result_table[(0 + 1) * num_cols + 0], result_table[(0 + 1) * num_cols + 1]);  // Sends uid to client
        dyad_addListener(client->stream, DYAD_EVENT_DATA, postAuthData, NULL);
        handleChatsUpdate(client->uid);
    } else if (client && !result_table) {
        printf("FAILED\n");
        dyad_writef(client->stream, "/@/auth_answer|-1");
    } else if (client) {
        dyad_writef(client->stream, "/@/auth_answer|-1");
    }
    
    sqlite3_free_table(result_table);
    free(username);
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

//void createChat()

int main() {
#if defined(__linux__)
    setvbuf(stdout, NULL, _IONBF, 0);
#endif
    char *result;

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
    register_user("Pomogite", "memo", "testpas");
    register_user("Lel", "mem2", "testpas");
    register_user("Lel", "mem3", "testpas");
    register_user("Lel", "mem3", "testpas");
    register_user("Lel", "mem4", "testpas");
    char *str = malloc(196);
    sprintf(str, "INSERT INTO chats(is_group) VALUES('%d');", false);
    db_exec(db, str, NULL);
    free(str);
    str = NULL;

    // TODO remove this shit
    db_exec(db, "INSERT INTO participants(uid, chat_id) VALUES('1', '1');", NULL);
    db_exec(db, "INSERT INTO participants(uid, chat_id) VALUES('2', '1');", NULL);
    db_exec(db, "INSERT INTO participants(uid, chat_id) VALUES('3', '1');", NULL);
    db_exec(db, "INSERT INTO participants(uid, chat_id) VALUES('4', '1');", NULL);
    //handleChatsUpdate("/@1/getmsg");

    while (dyad_getStreamCount() > 0) {
        dyad_update();
        check_disconnected_client();
    }

    dyad_shutdown();
    sqlite3_close(db);
    return 0;
}

void signal_handler(int signal_number) {
    (void)signal_number;
    /* TODO: Put exit cleanup code here. */
    printf("Bye-bye!\n");
    dyad_shutdown();
    exit(EXIT_SUCCESS);
}

//TODO List
// 1. Add user
// 2. Stickers
// 3. Files
