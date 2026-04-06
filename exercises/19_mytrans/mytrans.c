// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
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

  while (fgets(line, sizeof(line), file)) {
      if (line[0] == '#') {
          if (current_word[0] != '\0') {
              hash_table_insert(table, current_word, current_translation);
              (*dict_count)++;
          }
          strncpy(current_word, line + 1, sizeof(current_word) - 1);
          trim(current_word);
          current_translation[0] = '\0';
      } else {
          strncat(current_translation, line, sizeof(current_translation) - strlen(current_translation) - 1);
      }
  }
  if (current_word[0] != '\0') {
      hash_table_insert(table, current_word, current_translation);
      (*dict_count)++;
  }

  fclose(file);
  return 0;
}
