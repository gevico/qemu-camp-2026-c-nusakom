#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 1024  // 哈希表大小

// 哈希表节点结构
typedef struct HashNode {
    char *word;
    int count;
    struct HashNode *next;
} HashNode;

// 哈希表结构
typedef struct {
    HashNode **table;
    int size;
} HashTable;

// djb2哈希函数
unsigned long djb2_hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

// 创建哈希表
HashTable *create_hash_table(int size) {
    HashTable *ht = malloc(sizeof(HashTable));
    if (!ht) return NULL;
    ht->table = calloc(size, sizeof(HashNode *));
    if (!ht->table) {
        free(ht);
        return NULL;
    }
    ht->size = size;
    return ht;
}

// 向哈希表中插入单词
void hash_table_insert(HashTable *ht, const char *word) {
    unsigned long hash = djb2_hash(word) % ht->size;

    HashNode *node = ht->table[hash];
    while (node) {
        if (strcmp(node->word, word) == 0) {
            node->count++;
            return;
        }
        node = node->next;
    }
    HashNode *new_node = malloc(sizeof(HashNode));
    if (!new_node) return; // 内存分配失败处理
    new_node->word = strdup(word);
    if (!new_node->word) {
        free(new_node);
        return;
    }
    new_node->count = 1;
    new_node->next = ht->table[hash];
    ht->table[hash] = new_node;
}

// 从哈希表中获取所有单词及其计数
// 注意：调用者必须确保 nodes 数组足够大
void get_all_words(HashTable *ht, HashNode **nodes, int *count) {
    *count = 0;
    for (int i = 0; i < ht->size; i++) {
        HashNode *node = ht->table[i];
        while (node) {
            nodes[(*count)++] = node;
            node = node->next;
        }
    }
}

// 比较函数用于排序
int compare_nodes(const void *a, const void *b) {
    HashNode *node_a = *(HashNode **)a;
    HashNode *node_b = *(HashNode **)b;
    
    // 先按计数降序，再按字母升序
    if (node_b->count != node_a->count)
        return node_b->count - node_a->count;
    return strcmp(node_a->word, node_b->word);
}

// 释放哈希表内存
void free_hash_table(HashTable *ht) {
    if (!ht) return;
    for (int i = 0; i < ht->size; i++) {
        HashNode *node = ht->table[i];
        while (node != NULL) {
            HashNode *temp = node;
            node = node->next;
            free(temp->word);
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}

// 从字符串中获取下一个单词
char *get_next_word(const char **text) {
    const char *p = *text;
    // 跳过非字母字符
    while (*p && !isalpha((unsigned char)*p)) p++;
    if (!*p) { 
        *text = p; 
        return NULL; 
    }
    const char *start = p;
    while (*p && isalpha((unsigned char)*p)) p++;
    int len = p - start;
    char *word = malloc(len + 1);
    if (!word) return NULL;
    strncpy(word, start, len);
    word[len] = '\0';
    // 转小写
    for (int i = 0; i < len; i++) word[i] = tolower((unsigned char)word[i]);
    *text = p;
    return word;
}

int main(int argc, char *argv[]) {
    const char* file_path = "./paper.txt";

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("无法打开文件");
        return 1;
    }

    HashTable *ht = create_hash_table(TABLE_SIZE);
    if (!ht) {
        fclose(file);
        return 1;
    }

    char buffer[4096];
    
    // 从文件读取直到EOF
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        const char *ptr = buffer;
        char *word;
        
        while ((word = get_next_word(&ptr)) != NULL) {
            hash_table_insert(ht, word);
            free(word);
        }
    }
    
    fclose(file);
    
    // 【关键修复】先统计总节点数，再分配内存
    int total_nodes = 0;
    for (int i = 0; i < ht->size; i++) {
        HashNode *node = ht->table[i];
        while (node) {
            total_nodes++;
            node = node->next;
        }
    }

    HashNode **nodes = NULL;
    if (total_nodes > 0) {
        nodes = malloc(total_nodes * sizeof(HashNode *));
        if (!nodes) {
            fprintf(stderr, "Memory allocation failed for nodes array\n");
            free_hash_table(ht);
            return 1;
        }
    } else {
        // 没有单词，直接输出空结果
        printf("\n单词统计结果（按频率降序排列）:\n");
        printf("%-20s %s\n", "单词", "出现次数");
        printf("-------------------- ----------\n");
        free_hash_table(ht);
        return 0;
    }

    // 收集所有单词节点
    int node_count = 0;
    get_all_words(ht, nodes, &node_count);
    
    // 排序
    qsort(nodes, node_count, sizeof(HashNode *), compare_nodes);
    
    // 输出结果
    printf("\n单词统计结果（按频率降序排列）:\n");
    printf("%-20s %s\n", "单词", "出现次数");
    printf("-------------------- ----------\n");
    for (int i = 0; i < node_count; i++) {
        printf("%s:%d\n", nodes[i]->word, nodes[i]->count);
    }
    
    // 释放内存
    free(nodes);
    free_hash_table(ht);
    
    return 0;
}