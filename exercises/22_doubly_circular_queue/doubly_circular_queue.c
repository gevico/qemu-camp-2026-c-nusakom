#include "doubly_circular_queue.h"

#include <stdlib.h>

// 头尾哨兵
static struct node tailsentinel;
static struct node headsentinel = {0, NULL, &tailsentinel};
static struct node tailsentinel = {0, &headsentinel, NULL};

static link head = &headsentinel;
static link tail = &tailsentinel;

link make_node(int data) {
    link p = malloc(sizeof(struct node));
    p->item = data;
    p->prev = p->next = NULL;
    return p;
}

void free_node(link p) {
    free(p);
}

link search(int key) {
    link p;
    for (p = head->next; p != tail; p = p->next)
        if (p->item == key)
            return p;
    return NULL;
}

void insert(link p) {
    p->next = head->next;
    head->next->prev = p;
    head->next = p;
    p->prev = head;
}

void delete(link p) {
    p->prev->next = p->next;
    p->next->prev = p->prev;
}

void traverse(void (*visit)(link)) {
    link p;
    for (p = head->next; p != tail; p = p->next)
        visit(p);
}

void destroy(void) {
    link q, p = head->next;
    head->next = tail;
    tail->prev = head;
    while (p != tail) {
        q = p;
        p = p->next;
        free_node(q);
    }
}
