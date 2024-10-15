#ifndef PARSER_H
#define PARSER_H

#include <unistd.h>  // For pid_t

#define MAX_ARGS 128
#define MAX_PIPES 10

typedef struct cmdline {
    char *args[MAX_ARGS];
    int background;
    char *input_file;
    char *output_file;
    pid_t pid;
    char *command;  // New field to store the command name
} cmdline;


cmdline *parse_cmd(char *input);
int execute_cmd(cmdline *cmd);
int count_pipes(char *cmd);
void split_piped_cmds(char *cmd, char **piped_cmds);
int execute_multiple_pipes(char **piped_cmds, int num_pipes);
void list_jobs();
void wait_for_jobs();

#endif

