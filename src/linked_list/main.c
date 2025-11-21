#include <stdio.h>

#include "linked_list.h"

static void print_list(const struct linked_list *list)
{
    const struct list_node *node = NULL;

    if (list == NULL) {
        printf("[ ]\n");
        return;
    }

    node = list->head;
    printf("[");
    while (node != NULL) {
        printf("%d", node->value);
        if (node->next != NULL) {
            printf(" -> ");
        }
        node = node->next;
    }
    printf("]\n");
}

int main(void)
{
    struct linked_list numbers;
    linked_list_init(&numbers);

    printf("Start with an empty list:\n");
    print_list(&numbers);

    printf("\nAppend a few values to the back:\n");
    linked_list_push_back(&numbers, 10);
    linked_list_push_back(&numbers, 20);
    linked_list_push_back(&numbers, 30);
    print_list(&numbers);

    printf("\nInsert a value at the front:\n");
    linked_list_push_front(&numbers, 5);
    print_list(&numbers);

    printf("\nFind the first node that contains 20:\n");
    const struct list_node *found = linked_list_find(&numbers, 20);
    if (found != NULL) {
        printf("Found value %d at address %p\n", found->value, (const void *)found);
    } else {
        printf("Value not present\n");
    }

    printf("\nRemove the first node with value 20:\n");
    linked_list_remove_first(&numbers, 20);
    print_list(&numbers);

    printf("\nAttempt to remove a value that is not present (99):\n");
    if (!linked_list_remove_first(&numbers, 99)) {
        printf("99 was not found; list unchanged.\n");
    }
    print_list(&numbers);

    printf("\nList length is now %zu\n", linked_list_length(&numbers));

    printf("\nClear the list to free all nodes:\n");
    linked_list_clear(&numbers);
    print_list(&numbers);

    return 0;
}
