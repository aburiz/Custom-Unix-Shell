#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"

#define MAX_CMD_LEN 1024

int main() {
    char cmd[MAX_CMD_LEN];           // Buffer to store the input command
    char *piped_cmds[MAX_PIPES];     // Array to store piped commands (if any)

    while (1) {
        printf("my_shell> ");        // Display shell prompt

        // Read the user input and check for end-of-file (EOF)
        if (!fgets(cmd, sizeof(cmd), stdin)) {
            break;  // Exit the loop if EOF (Ctrl+D)
        }

        // Remove the newline at the end of the command
        cmd[strlen(cmd) - 1] = '\0';  // Ensure there's no newline at the end

        // Handle the 'exit' built-in command
        if (strcmp(cmd, "exit") == 0) {
            break;  // Exit the shell loop if the command is 'exit'
        }

        // Built-in command: 'jobs'
        if (strcmp(cmd, "jobs") == 0) {
            list_jobs();              // Display background jobs
            continue;                 // Go back to the prompt after execution
        } 
        // Built-in command: 'wait'
        else if (strcmp(cmd, "wait") == 0) {
            wait_for_jobs();          // Wait for all background jobs to finish
            continue;                 // Go back to the prompt
        }

        // Count the number of pipes in the input command
        int num_pipes = count_pipes(cmd);  // Count how many pipes ('|') are present

        // No pipes detected, execute the command normally
        if (num_pipes == 0) {
            cmdline *parsed_cmd = parse_cmd(cmd);  // Parse the command into a structure
            execute_cmd(parsed_cmd);               // Execute the parsed command
        } 
        // Handle commands with pipes
        else {
            split_piped_cmds(cmd, piped_cmds);     // Split command into individual piped segments
            execute_multiple_pipes(piped_cmds, num_pipes);  // Execute the commands with piping
        }
    }

    return 0;  // Exit the shell successfully
}
