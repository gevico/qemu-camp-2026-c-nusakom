#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h> // 需要 include ctype.h 用于 isspace

#include "common.h"

#define MAX_INPUT 1024
#define MAX_ARGS 64

// ======================
// 自定义命令系统
// ======================

typedef int (*cmd_func_0_t)(void);
typedef int (*cmd_func_1_t)(const char*);
typedef int (*cmd_func_2_t)(const char*, const char*);

typedef struct {
    const char *name;
    int is_arg_required;
    union {
        cmd_func_0_t func_0;
        cmd_func_1_t func_1;
        cmd_func_2_t func_2;
    } func;
} Command;

Command commands[] = {
    {"myfile", 1, .func.func_1 = __cmd_myfile},
    {"mysed",  2, .func.func_2 = __cmd_mysed},
    {"mytrans", 1, .func.func_1 = __cmd_mytrans},
    {"mywc", 1, .func.func_1 = __cmd_mywc},
    {NULL, 0, .func.func_0 = NULL}
};

// ======================
// 原有内置命令 & 工具函数
// ======================

void execute_cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "mybash: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("mybash");
    }
  }
}

void execute_exit() { exit(0); }

int is_builtin_command(char **args) {
  if (!args || !args[0]) return 0;

  if (strcmp(args[0], "exit") == 0) {
    execute_exit();
    return 1; // 已处理
  }
  
  if (strcmp(args[0], "cd") == 0) {
    execute_cd(args);
    return 1; // 已处理
  }

  return 0; // 不是内置命令
}

int parse_input(char *input, char **args) {
  int i = 0;
  char *buf = input;
  char arg_buf[MAX_INPUT]; 
  int arg_buf_idx = 0;
  int in_quotes = 0;

  // 跳过开头的空白字符
  while (*buf && isspace((unsigned char)*buf)) buf++;

  if (*buf == '\0') return 0; // 空行或只有空格

  while (*buf != '\0' && i < MAX_ARGS - 1) {
      char c = *buf;

      // 处理引号
      if (c == '"' || c == '\'') {
          if (!in_quotes) {
              in_quotes = 1;
              buf++;
              continue;
          } else {
              // 结束引号
              in_quotes = 0;
              buf++;
              continue;
          }
      }

      // 如果是空格且不在引号内，视为分隔符
      if (!in_quotes && isspace((unsigned char)c)) {
          if (arg_buf_idx > 0) {
              arg_buf[arg_buf_idx] = '\0';
              args[i++] = strdup(arg_buf);
              arg_buf_idx = 0;
              memset(arg_buf, 0, sizeof(arg_buf));
          }
          buf++;
          continue;
      }

      // 收集字符
      if (arg_buf_idx < MAX_INPUT - 1) {
          arg_buf[arg_buf_idx++] = c;
      }
      
      buf++;
  }

  // 处理最后一个参数
  if (arg_buf_idx > 0) {
      arg_buf[arg_buf_idx] = '\0';
      args[i++] = strdup(arg_buf);
  }

  args[i] = NULL; // NULL结尾
  return i;
}

// ======================
// 主函数
// ======================

int main(int argc, char *argv[]) {
  char input[MAX_INPUT];
  char *args[MAX_ARGS];

  // 初始化 args 数组（可选，但更安全）
  for(int k=0; k<MAX_ARGS; k++) args[k] = NULL;

  if (argc > 1) {
    const char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (!file) {
      printf("mybash: cannot open file: %s\n", filename);
      return 1;
    }

    printf("mybash: reading commands from file '%s'\n", filename);

    while (fgets(input, sizeof(input), file)) {
      input[strcspn(input, "\n")] = '\0'; // 移除换行符

      // 清空之前的 args (防止残留)
      for(int k=0; k<MAX_ARGS; k++) {
          if(args[k]) free(args[k]);
          args[k] = NULL;
      }

      int argc_parsed = parse_input(input, args);

      if (argc_parsed == 0) {
        continue;
      }

      // 处理内置命令
      if (is_builtin_command(args)) {
        // 注意：execute_exit 已经调用了 exit(0)，所以这里不需要 continue
        // 但为了安全，如果是 cd 则继续
        continue;
      }

      const char *cmd_name = args[0];
      const char *cmd_arg1 = (argc_parsed >= 2) ? args[1] : NULL;
      const char *cmd_arg2 = (argc_parsed >= 3) ? args[2] : NULL;

      int found = 0;
      for (Command *cmd = commands; cmd->name != NULL; cmd++) {
        if (strcmp(cmd_name, cmd->name) == 0) {
          found = 1;
          if (cmd->is_arg_required == 0) {
            cmd->func.func_0();
          } else if (cmd->is_arg_required == 1) {
            cmd->func.func_1(cmd_arg1);
          } else if (cmd->is_arg_required == 2) {
            cmd->func.func_2(cmd_arg1, cmd_arg2); // 修复：使用正确的参数
          }
          break;
        }
      }

      if (!found) {
        fprintf(stderr, "mybash: command not found: %s\n", cmd_name);
      }
      
      // 清理本次循环分配的内存
      for(int k=0; k<argc_parsed; k++) {
          if(args[k]) free(args[k]);
      }
    }

    fclose(file);
    return 0;
  } 
  else {
    while (1) {
      printf("mybash$ ");
      fflush(stdout);

      if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("\n");
        break;
      }

      input[strcspn(input, "\n")] = '\0';

      // 清空 args
      for(int k=0; k<MAX_ARGS; k++) {
          if(args[k]) free(args[k]);
          args[k] = NULL;
      }

      int argc = parse_input(input, args);

      if (argc == 0) {
        continue;
      }

      if (is_builtin_command(args)) {
        continue;
      }

      const char *cmd_name = args[0];
      const char *cmd_arg = (argc >= 2) ? args[1] : NULL;
      const char *cmd_arg2 = (argc >= 3) ? args[2] : NULL; // 修复：正确获取第二个参数

      int found = 0;
      for (Command *cmd = commands; cmd->name != NULL; cmd++) {
        if (strcmp(cmd_name, cmd->name) == 0) {
          found = 1;
          if (cmd->is_arg_required == 0) {
            cmd->func.func_0();
          } else if (cmd->is_arg_required == 1) {
            cmd->func.func_1(cmd_arg);
          } else if (cmd->is_arg_required == 2) {
            cmd->func.func_2(cmd_arg, cmd_arg2); // 修复：使用 cmd_arg2
          }
          break;
        }
      }

      if (!found) {
        fprintf(stderr, "mybash: command not found: %s\n", cmd_name);
      }

      // 清理内存
      for(int k=0; k<argc; k++) {
          if(args[k]) free(args[k]);
      }
    }
  }

  return 0;
}