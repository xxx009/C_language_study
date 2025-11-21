#ifndef CLANG_STUDY_LINKED_LIST_H
#define CLANG_STUDY_LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>

struct list_node {
    int value;
    struct list_node *next;
};

struct linked_list {
    struct list_node *head;
    size_t length;
};

void linked_list_init(struct linked_list *list);
size_t linked_list_length(const struct linked_list *list);
bool linked_list_push_front(struct linked_list *list, int value);
bool linked_list_push_back(struct linked_list *list, int value);
struct list_node *linked_list_find(const struct linked_list *list, int value);
bool linked_list_remove_first(struct linked_list *list, int value);
void linked_list_clear(struct linked_list *list);

#endif /* CLANG_STUDY_LINKED_LIST_H */
