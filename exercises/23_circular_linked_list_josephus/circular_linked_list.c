#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    if (n <= 0) return NULL;
    Node *head = NULL, *tail = NULL;
    for (int i = 1; i <= n; i++) {
        Node *node = malloc(sizeof(Node));
        node->id = i;
        node->next = NULL;
        if (!head) head = node;
        else tail->next = node;
        tail = node;
    }
    tail->next = head; /* 形成环 */
    return head;
}

void free_list(Node* head) {
    if (!head) return;
    Node *p = head;
    do {
        Node *next = p->next;
        free(p);
        p = next;
    } while (p != head);
}
