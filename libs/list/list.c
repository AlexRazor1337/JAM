#include "list.h"
// t_list *mx_sort_list(t_list *lst, bool (*cmp)(void*, void*));

t_list *mx_create_node(void *data) {
    t_list *newNode = (t_list*)malloc(sizeof(newNode));
    newNode -> next = NULL;
    newNode -> data = data;
    return newNode;
}


int mx_list_size(t_list *list) {
    int count = 0;

    while (list) {
        list = list->next;
        count++;
    }
    return count;
}


void mx_push_back(t_list **list, void *data) {
    if (!list) {
        return;
    }
    if (!*list) {
        *list = mx_create_node(data);
        return;
    }
    t_list *temp = *list;
    while (temp -> next) {
        temp = temp -> next;
    }


    temp -> next = mx_create_node(data);
    printf("%s PUSH %s %s %s\n", "\x1B[31m", data, "\x1B[32m", temp -> next -> data);
}

void mx_push_front(t_list **list, void *data) {
    if (!list) {
        return;
    }
    if (!*list) {
        *list = mx_create_node(data);
        return;
    }
    t_list *temp = mx_create_node(data);
    temp -> next = *list;
    *list = temp;
}


void mx_pop_back(t_list **list) {
    if (list == NULL || *list == NULL) {
        return;
    }
    if ((*list) -> next == NULL) {
        free(*list);
        *list = NULL;
        return;
    }
    t_list *temp = *list;
    while (temp && temp -> next -> next) {
        temp = temp -> next;
    }
    free(temp -> next);
    temp -> next = NULL;
}


void mx_pop_front(t_list **list) {
    if (list == NULL || *list == NULL) {
        return;
    }
    if ((*list) -> next == NULL) {
        free(*list);
        *list = NULL;
        return;
    }
    t_list *temp = (*list) -> next;
    free(*list);
    *list = temp;
}


void mx_pop_index(t_list **list, int index) {
    if (!*list) return;
    t_list *temp = *list, *helper = NULL;
    if (index < 1 || temp -> next == NULL) {
        *list = temp -> next;
        free(temp);
        temp = NULL;
        return;
    }
    for (int i = 0; i < index; i++) {
        if (temp -> next == NULL)
            break;
        helper = temp;
        temp = temp -> next;
    }
    helper -> next = temp -> next;
    free(temp);
    temp = NULL;
    return;
}
