// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    /* 去除末尾空白 */
    int len = strlen(str);
    while (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r' || str[len-1] == ' '))
        str[--len] = '\0';
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};
  int in_entry = 0;

    // 解析词典格式：#word\nTrans:translation\n
    while (fgets(line, sizeof(line), file)) {
        trim(line);
        if (line[0] == '#') {
            strncpy(current_word, line + 1, sizeof(current_word) - 1);
            in_entry = 1;
        } else if (in_entry && strncmp(line, "Trans:", 6) == 0) {
            strncpy(current_translation, line + 6, sizeof(current_translation) - 1);
            hash_table_insert(table, current_word, current_translation);
            if (dict_count) (*dict_count)++;
            in_entry = 0;
        }
    }

  fclose(file);
  return 0;
}
