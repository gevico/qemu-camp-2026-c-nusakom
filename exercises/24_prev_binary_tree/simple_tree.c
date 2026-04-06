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
    QueueNode *node = malloc(sizeof(QueueNode));
    node->tree_node = tree_node;
    node->next = NULL;
    if (q->rear) q->rear->next = node;
    else q->front = node;
    q->rear = node;
}

TreeNode* dequeue(Queue *q) {
    if (!q->front) return NULL;
    QueueNode *node = q->front;
    TreeNode *t = node->tree_node;
    q->front = node->next;
    if (!q->front) q->rear = NULL;
    free(node);
    return t;
}

bool is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) dequeue(q);
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    if (size == 0) return NULL;
    if (level_order[0] == INT_MIN) return NULL;

    TreeNode *root = malloc(sizeof(TreeNode));
    root->val = level_order[0];
    root->left = root->right = NULL;

    Queue *q = create_queue();
    enqueue(q, root);
    int i = 1;

    while (!is_empty(q) && i < size) {
        TreeNode *node = dequeue(q);
        /* left child */
        if (i < size) {
            if (level_order[i] != INT_MIN) {
                node->left = malloc(sizeof(TreeNode));
                node->left->val = level_order[i];
                node->left->left = node->left->right = NULL;
                enqueue(q, node->left);
            }
            i++;
        }
        /* right child */
        if (i < size) {
            if (level_order[i] != INT_MIN) {
                node->right = malloc(sizeof(TreeNode));
                node->right->val = level_order[i];
                node->right->left = node->right->right = NULL;
                enqueue(q, node->right);
            }
            i++;
        }
    }

    free_queue(q);
    return root;
}

void preorder_traversal(TreeNode *root) {
    if (!root) return;
    printf("%d ", root->val);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    if (!root) return;
    /* 用数组模拟栈 */
    TreeNode *stack[1024];
    int top = 0;
    stack[top++] = root;
    while (top > 0) {
        TreeNode *node = stack[--top];
        printf("%d ", node->val);
        if (node->right) stack[top++] = node->right;
        if (node->left)  stack[top++] = node->left;
    }
}

void free_tree(TreeNode *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
