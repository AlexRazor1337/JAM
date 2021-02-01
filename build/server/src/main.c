#include "../inc/header.h"


void db_prepare(sqlite3 *db) {
    db_exec(db, "CREATE TABLE IF NOT EXISTS users (id integer PRIMARY KEY AUTOINCREMENT, name varchar, login varchar, password varchar, last_visit datetime);", NULL);
    db_exec(db, "CREATE TABLE IF NOT EXISTS messages (id_from integer, id_to integer, text text, timestamp datetime, attachment integer);", NULL);
    db_exec(db, "CREATE TABLE IF NOT EXISTS files (id integer PRIMARY KEY AUTOINCREMENT, hash varchar, name varchar, extension varchar);", NULL);
}


static void onData(dyad_Event *e) {
    dyad_write(e->stream, e->data, e->size);
}

static void onAccept(dyad_Event *e) {
    dyad_addListener(e->remote, DYAD_EVENT_DATA, onData, NULL);
    dyad_writef(e->remote, "Echo server\r\n");
}


int main() {
#if defined(__linux__)
    setvbuf(stdout, NULL, _IONBF, 0);
#endif
    
    sqlite3 *db;
    const unsigned char* result;
    
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
    db_prepare(db);
#pragma endregion db_init
    
    db_exec(db, "SELECT SQLITE_VERSION()", &result);
    printf("VER. %s\n", result);
    
#pragma region sockets_init
    dyad_init();
    printf("INITIALIZING DYAD: VER. %s\n", dyad_getVersion());
    
    dyad_Stream *serv = dyad_newStream();
    dyad_addListener(serv, DYAD_EVENT_ACCEPT, onAccept, NULL);
    dyad_listen(serv, 8000);
    
    signal(SIGINT, signal_handler);
#pragma endregion sockets_init
    
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
    exit(0);
}
