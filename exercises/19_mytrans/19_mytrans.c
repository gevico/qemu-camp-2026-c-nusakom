#include "myhash.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void to_lowercase(char *str) {
    for (; *str; ++str)
        *str = tolower((unsigned char)*str);
}

int main(int argc, char *argv[]) {
    const char *dict_path = "./dict.txt";
    const char *input_path = "./text.txt";

    HashTable *table = create_hash_table();
    if (!table) {
        fprintf(stderr, "无法创建哈希表\n");
        return 1;
    }

    uint64_t dict_count = 0;
    if (load_dictionary(dict_path, table, &dict_count) != 0) {
        fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
        free_hash_table(table);
        return 1;
    }

    FILE *file = fopen(input_path, "r");
    if (!file) {
        fprintf(stderr, "无法打开输入文件: %s\n", input_path);
        free_hash_table(table);
        return 1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        char *word = strtok(line, " \t");
        while (word != NULL) {
            char lower_word[256];
            strncpy(lower_word, word, sizeof(lower_word) - 1);
            lower_word[sizeof(lower_word) - 1] = '\0';
            to_lowercase(lower_word);

            const char *translation = hash_table_lookup(table, lower_word);
            printf("原文: %s\t", word);
            if (translation) {
                printf("翻译: %s\n", translation);
            } else {
                printf("未找到该单词的翻译。\n");
            }
            word = strtok(NULL, " \t");
        }
    }

    fclose(file);
    free_hash_table(table);
    return 0;
}
