#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "common.h"

#define MAX_INPUT 1024
#define MAX_ARGS 64

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
    {"myfile",  1, .func.func_1 = __cmd_myfile},
    {"mysed",   2, .func.func_2 = __cmd_mysed},
    {"mytrans", 1, .func.func_1 = __cmd_mytrans},
    {"mywc",    1, .func.func_1 = __cmd_mywc},
    {NULL, 0, .func.func_0 = NULL}
};

void execute_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "mybash: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) perror("mybash");
    }
}

void execute_exit(void) { exit(0); }

int is_builtin_command(char **args) {
    if (args[0] == NULL) return 0;
    if (strcmp(args[0], "cd") == 0)   { execute_cd(args); return 1; }
    if (strcmp(args[0], "exit") == 0) { execute_exit();   return 1; }
    return 0;
}

int parse_input(char *input, char **args) {
    int i = 0, in_quotes = 0, arg_buf_idx = 0;
    char *buf = input;
    char arg_buf[MAX_INPUT];
    memset(arg_buf, 0, sizeof(arg_buf));

    while (*buf != '\0' && i < MAX_ARGS - 1) {
        char c = *buf;
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (c == ' ' && !in_quotes) {
            if (arg_buf_idx > 0) {
                arg_buf[arg_buf_idx] = '\0';
                args[i++] = strdup(arg_buf);
                arg_buf_idx = 0;
                memset(arg_buf, 0, sizeof(arg_buf));
            }
        } else {
            if (arg_buf_idx < MAX_INPUT - 1)
                arg_buf[arg_buf_idx++] = c;
        }
        buf++;
    }
    if (arg_buf_idx > 0) {
        arg_buf[arg_buf_idx] = '\0';
        args[i++] = strdup(arg_buf);
    }
    args[i] = NULL;
    return i;
}

static void dispatch(char **args, int argc_parsed) {
    if (is_builtin_command(args)) return;

    const char *cmd_name = args[0];
    const char *arg1 = (argc_parsed >= 2) ? args[1] : NULL;
    const char *arg2 = (argc_parsed >= 3) ? args[2] : NULL;

    for (Command *cmd = commands; cmd->name != NULL; cmd++) {
        if (strcmp(cmd_name, cmd->name) == 0) {
            if      (cmd->is_arg_required == 0) cmd->func.func_0();
            else if (cmd->is_arg_required == 1) cmd->func.func_1(arg1);
            else if (cmd->is_arg_required == 2) cmd->func.func_2(arg1, arg2);
            return;
        }
    }
    fprintf(stderr, "mybash: command not found: %s\n", cmd_name);
}

int main(int argc, char *argv[]) {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            printf("mybash: cannot open file: %s\n", argv[1]);
            return 1;
        }
        printf("mybash: reading commands from file '%s'\n", argv[1]);
        while (fgets(input, sizeof(input), file)) {
            input[strcspn(input, "\n")] = '\0';
            int n = parse_input(input, args);
            if (n == 0) continue;
            dispatch(args, n);
            for (int k = 0; k < n; k++) { free(args[k]); args[k] = NULL; }
        }
        fclose(file);
    } else {
        while (1) {
            printf("mybash$ ");
            fflush(stdout);
            if (fgets(input, sizeof(input), stdin) == NULL) { printf("\n"); break; }
            input[strcspn(input, "\n")] = '\0';
            int n = parse_input(input, args);
            if (n == 0) continue;
            dispatch(args, n);
            for (int k = 0; k < n; k++) { free(args[k]); args[k] = NULL; }
        }
    }
    return 0;
}
