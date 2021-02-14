#include "../inc/header.h"

int db_exec(sqlite3 *db, char* querry, char** result) {
    sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(db, querry, -1, &res, 0);

    if (rc != SQLITE_OK) {
        syslog(0, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    rc = sqlite3_step(res);

    if (rc == SQLITE_ROW) {

        *result = strdup((char *)sqlite3_column_text(res, 0));
        sqlite3_finalize(res);
    }
    return rc;
}
