#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    if (n <= 0) return NULL;
    Node *head = malloc(sizeof(Node));
    head->id = 1;
    head->next = head;
    Node *curr = head;
    for (int i = 2; i <= n; i++) {
        Node *new_node = malloc(sizeof(Node));
        new_node->id = i;
        new_node->next = head;
        curr->next = new_node;
        curr = new_node;
    }
    return head;
}

void free_list(Node* head) {
    if (head == NULL) return;
    Node *curr = head->next;
    while (curr != head) {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(head);
}
