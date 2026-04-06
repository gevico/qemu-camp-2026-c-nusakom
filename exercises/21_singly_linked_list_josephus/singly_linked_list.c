#include "singly_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

static link head = NULL;

link make_node(unsigned char item) {
    link p = malloc(sizeof(struct node));
    p->item = item;
    p->next = NULL;
    return p;
}

void free_node(link p) { free(p); }

link search(unsigned char key) {
    link p = head;
    while (p) {
        if (p->item == key) return p;
        p = p->next;
    }
    return NULL;
}

void insert(link p) {
    p->next = head;
    head = p;
}

void delete(link p) {
    if (!head) return;
    if (head == p) { head = head->next; return; }
    link prev = head;
    while (prev->next && prev->next != p) prev = prev->next;
    if (prev->next) prev->next = p->next;
}

void traverse(void (*visit)(link)) {
    link p = head;
    while (p) { visit(p); p = p->next; }
}

void destroy(void) {
    link p = head;
    while (p) { link next = p->next; free(p); p = next; }
    head = NULL;
}

void push(link p) {
    p->next = head;
    head = p;
}

link pop(void) {
    if (!head) return NULL;
    link p = head;
    head = head->next;
    return p;
}

void free_list(link list_head) {
    while (list_head) {
        link next = list_head->next;
        free(list_head);
        list_head = next;
    }
}
