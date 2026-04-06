#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, TreeNode *tree_node) {
    QueueNode *new_node = malloc(sizeof(QueueNode));
    new_node->tree_node = tree_node;
    new_node->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = new_node;
        return;
    }
    q->rear->next = new_node;
    q->rear = new_node;
}

TreeNode* dequeue(Queue *q) {
    if (q->front == NULL) return NULL;
    QueueNode *temp = q->front;
    TreeNode *tree_node = temp->tree_node;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return tree_node;
}

bool is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    if (size == 0 || level_order[0] == -1) return NULL;
    TreeNode *root = malloc(sizeof(TreeNode));
    root->data = level_order[0];
    root->left = root->right = NULL;
    Queue *q = create_queue();
    enqueue(q, root);
    int i = 1;
    while (i < size) {
        TreeNode *curr = dequeue(q);
        if (level_order[i] != -1) {
            curr->left = malloc(sizeof(TreeNode));
            curr->left->data = level_order[i];
            curr->left->left = curr->left->right = NULL;
            enqueue(q, curr->left);
        }
        i++;
        if (i < size && level_order[i] != -1) {
            curr->right = malloc(sizeof(TreeNode));
            curr->right->data = level_order[i];
            curr->right->left = curr->right->right = NULL;
            enqueue(q, curr->right);
        }
        i++;
    }
    free_queue(q);
    return root;
}

void preorder_traversal(TreeNode *root) {
    if (root == NULL) return;
    printf("%d ", root->data);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    if (root == NULL) return;
    TreeNode *stack[100];
    int top = -1;
    stack[++top] = root;
    while (top >= 0) {
        TreeNode *curr = stack[top--];
        printf("%d ", curr->data);
        if (curr->right) stack[++top] = curr->right;
        if (curr->left) stack[++top] = curr->left;
    }
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
