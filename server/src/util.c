#include "../inc/header.h"

int generate_unique_id(t_list *connections) {
    int id;

    while (true) {
        id = rand();
        t_list *carret = connections;
        while (carret) {
            t_connection *connection = carret->data;
            if (connection->id == id) continue;
            carret = carret->next;
        }
        break;
    }

    return id;
}

t_connection *find_node(int id, t_list *connections) {
    t_list *carret = connections;

    while (carret) {
        t_connection *data = carret->data;
        if (data->id == id) return data;
        carret = carret->next;
    }
    return NULL;
}

t_connection *find_node_uid(int uid, t_list *connections) {
    t_list *carret = connections;
    while (carret) {
        t_connection *data = carret->data;
        if (data->uid == uid) return data;
        carret = carret->next;
    }
    return NULL;
}


char *mx_itoa(int n) { // doesn't support negative numbers
    if (n == 0) return strdup("0");

    int len = 0;

    for (int i = n; i > 0 || i < 0; i /= 10) len++;

    char *number = malloc(len);
    int cnt = 0;

    while(cnt < len) {
        number[len - cnt - 1] = abs(n % 10) + '0';
        n /= 10;
        cnt++;
    }

    return number;
}

char *jsonlist_from_jsones(vec_str_t v, int bsize) {
    char *final_json = malloc(bsize);
    char *temporal = malloc(bsize);
    char *cursor = temporal;
    char *cursor_2 = final_json;
    if (v.length == 1) {
        free(temporal);
        free(final_json);
        return NULL;
    }
    printf("TE: %s\n", v.data[0]);
    sprintf(final_json, "[%s", (char *)v.data[0]);
    int i; char *val;
    vec_foreach(&v, val, i) {
         printf("CE: %s\n", v.data[i]);
        if (i > 0) {
            if (i == v.length - 1) {
                sprintf(cursor, "%s,%s]", cursor_2, (char *)v.data[i]);
                free(cursor_2);
                cursor_2 = NULL;
            } else {
                sprintf(cursor, "%s,%s", cursor_2, (char *)v.data[i]);
                free(cursor_2);
                cursor_2 = malloc(bsize);
                char *temp = cursor_2;
                cursor_2 = cursor;
                cursor = temp;
            }
        }

    }

    return cursor;
}

t_connection *create_connection(dyad_Stream *stream, t_list *connections) {
    t_connection *new_connection = malloc(sizeof(t_connection));
    new_connection->stream = stream;
    new_connection->id = generate_unique_id(connections);
    return new_connection;
}

void strdel(char **str) {
    free(*str);
    *str = NULL;
}

void daemonize() {
    pid_t pid;

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    if (setsid() < 0) exit(EXIT_FAILURE);

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    printf("%d\n", pid);
    if (pid > 0) exit(EXIT_SUCCESS);
    umask(0);

    openlog ("uchat_server", LOG_PID, LOG_DAEMON);
}
