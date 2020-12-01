#include "../inc/header.h"

void db_prepare(sqlite3 *db) {
    db_exec(db, "CREATE TABLE IF NOT EXISTS users (id integer PRIMARY KEY AUTOINCREMENT, name varchar, login varchar, password varchar, last_visit datetime);", NULL);
    db_exec(db, "CREATE TABLE IF NOT EXISTS messages (id_from integer, id_to integer, text text, timestamp datetime, attachment integer);", NULL);
    db_exec(db, "CREATE TABLE IF NOT EXISTS files (id integer PRIMARY KEY AUTOINCREMENT, hash varchar, name varchar, extension varchar);", NULL);
}

int main() {
    sqlite3 *db;

    printf("INITIALIZING JAM SERVER:\n");
    printf("INITIALIZING SQLITE: ");
    {
        

        if (!is_dir_exists("server_data"))
            mkdir("server_data", 0755);

        int rc = sqlite3_open("server_data/main.db", &db);
    
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            exit(EXIT_FAILURE);
        }
        db_prepare(db);
    }


    const unsigned char* result;
    db_exec(db, "SELECT SQLITE_VERSION()", &result);
    printf("VER. %s\n", result);
    
    
    sqlite3_close(db);
    
    return 0;
}
