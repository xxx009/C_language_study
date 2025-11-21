#include "linked_list.h"

#include <stdlib.h>

void linked_list_init(struct linked_list *list)
{
    if (list == NULL) {
        return;
    }

    list->head = NULL;
    list->length = 0U;
}

size_t linked_list_length(const struct linked_list *list)
{
    if (list == NULL) {
        return 0U;
    }

    return list->length;
}

bool linked_list_push_front(struct linked_list *list, int value)
{
    if (list == NULL) {
        return false;
    }

    struct list_node *node = malloc(sizeof(*node));
    if (node == NULL) {
        return false;
    }

    node->value = value;
    node->next = list->head;
    list->head = node;
    list->length += 1U;
    return true;
}

bool linked_list_push_back(struct linked_list *list, int value)
{
    if (list == NULL) {
        return false;
    }

    struct list_node *node = malloc(sizeof(*node));
    if (node == NULL) {
        return false;
    }
    node->value = value;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
    } else {
        struct list_node *tail = list->head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = node;
    }

    list->length += 1U;
    return true;
}

struct list_node *linked_list_find(const struct linked_list *list, int value)
{
    if (list == NULL) {
        return NULL;
    }

    const struct list_node *current = list->head;
    while (current != NULL) {
        if (current->value == value) {
            /* Cast away const because API returns non-const for reuse. */
            return (struct list_node *)current;
        }
        current = current->next;
    }

    return NULL;
}

bool linked_list_remove_first(struct linked_list *list, int value)
{
    if (list == NULL) {
        return false;
    }

    struct list_node *previous = NULL;
    struct list_node *current = list->head;

    while (current != NULL && current->value != value) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return false;
    }

    if (previous == NULL) {
        list->head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current);
    list->length -= 1U;
    return true;
}

void linked_list_clear(struct linked_list *list)
{
    if (list == NULL) {
        return;
    }

    struct list_node *node = list->head;
    while (node != NULL) {
        struct list_node *next = node->next;
        free(node);
        node = next;
    }

    list->head = NULL;
    list->length = 0U;
}
