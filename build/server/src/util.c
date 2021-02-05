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
    //printf("GET UID: %d\n", uid);
    while (carret) {
        t_connection *data = carret->data;
        //printf("UID: %d %d\n", uid, data->uid);
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
