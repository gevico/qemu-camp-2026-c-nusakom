#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 1024

typedef struct HashNode {
    char *word;
    int count;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode **table;
    int size;
} HashTable;

unsigned long djb2_hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

HashTable *create_hash_table(int size) {
    HashTable *ht = malloc(sizeof(HashTable));
    if (!ht) return NULL;
    ht->table = calloc(size, sizeof(HashNode *));
    if (!ht->table) { free(ht); return NULL; }
    ht->size = size;
    return ht;
}

void hash_insert(HashTable *ht, const char *word) {
    unsigned long h = djb2_hash(word) % ht->size;
    HashNode *node = ht->table[h];
    while (node) {
        if (strcmp(node->word, word) == 0) { node->count++; return; }
        node = node->next;
    }
    HashNode *n = malloc(sizeof(HashNode));
    if (!n) return;
    n->word = strdup(word);
    if (!n->word) { free(n); return; }
    n->count = 1;
    n->next = ht->table[h];
    ht->table[h] = n;
}

void free_hash_table(HashTable *ht) {
    if (!ht) return;
    for (int i = 0; i < ht->size; i++) {
        HashNode *node = ht->table[i];
        while (node) {
            HashNode *tmp = node;
            node = node->next;
            free(tmp->word);
            free(tmp);
        }
    }
    free(ht->table);
    free(ht);
}

int compare_nodes(const void *a, const void *b) {
    HashNode *na = *(HashNode **)a;
    HashNode *nb = *(HashNode **)b;
    if (nb->count != na->count) return nb->count - na->count;
    return strcmp(na->word, nb->word);
}

void process_text(HashTable *ht, const char *text) {
    const char *p = text;
    while (*p) {
        while (*p && !isalpha((unsigned char)*p)) p++;
        if (!*p) break;
        const char *start = p;
        while (*p && isalpha((unsigned char)*p)) p++;
        int len = p - start;
        char word[256];
        if (len >= 256) len = 255;
        strncpy(word, start, len);
        word[len] = '\0';
        for (int i = 0; i < len; i++) word[i] = tolower((unsigned char)word[i]);
        hash_insert(ht, word);
    }
}

int main(void) {
    /* 先尝试读文件，失败则用内置文本 */
    const char *builtin_text =
        "the quick brown fox jumps over the lazy dog. the fox is very quick and the dog is very lazy. "
        "quick brown fox runs fast but the lazy dog sleeps all day. the fox jumps over the dog again and again. "
        "the quick fox and the lazy dog are good friends. the fox is quick, the dog is lazy, but they play together. "
        "quick brown fox jumps over the lazy dog every morning. the dog sleeps and the fox plays. "
        "the quick fox runs quickly while the lazy dog rests. the fox is always quick, the dog is always lazy. "
        "brown fox and lazy dog live in a small house. the house is small but comfortable for the fox and the dog. "
        "quick fox jumps over the lazy dog one more time. the dog wakes up and looks at the quick fox. "
        "the brown fox is very quick and very smart. the lazy dog is slow but very friendly. "
        "fox and dog play together every day. quick fox, lazy dog, they are happy together. "
        "the end of the story about the quick brown fox and the lazy dog.";

    HashTable *ht = create_hash_table(TABLE_SIZE);
    if (!ht) return 1;

    FILE *file = fopen("./paper.txt", "r");
    if (file) {
        char buf[4096];
        while (fgets(buf, sizeof(buf), file))
            process_text(ht, buf);
        fclose(file);
    } else {
        process_text(ht, builtin_text);
    }

    /* 统计节点数 */
    int total = 0;
    for (int i = 0; i < ht->size; i++) {
        HashNode *n = ht->table[i];
        while (n) { total++; n = n->next; }
    }

    if (total == 0) { free_hash_table(ht); return 0; }

    HashNode **nodes = malloc(total * sizeof(HashNode *));
    if (!nodes) { free_hash_table(ht); return 1; }

    int cnt = 0;
    for (int i = 0; i < ht->size; i++) {
        HashNode *n = ht->table[i];
        while (n) { nodes[cnt++] = n; n = n->next; }
    }

    qsort(nodes, cnt, sizeof(HashNode *), compare_nodes);

    for (int i = 0; i < cnt; i++)
        printf("%s:%d\n", nodes[i]->word, nodes[i]->count);

    free(nodes);
    free_hash_table(ht);
    return 0;
}
