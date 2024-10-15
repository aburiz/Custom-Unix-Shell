#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "parser.h"

// Global variables to store background jobs
cmdline *jobs[MAX_ARGS];  // Store cmdline pointers instead of pid_t
int job_count = 0;

cmdline *parse_cmd(char *input) {
    cmdline *cmd = malloc(sizeof(cmdline));  // Allocate memory for cmdline struct
    char *token;
    int i = 0;

    // Initialize cmdline structure (background flag, input/output redirection)
    cmd->background = 0;
    cmd->input_file = NULL;
    cmd->output_file = NULL;

    // Tokenize the input string by space
    token = strtok(input, " ");
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            cmd->background = 1;  // Background flag set if '&' found
        } else if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            cmd->input_file = token;  // Capture input file after '<'
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            cmd->output_file = token;  // Capture output file after '>'
        } else {
            cmd->args[i++] = token;  // Capture command and arguments
        }
        token = strtok(NULL, " ");
    }
    cmd->args[i] = NULL;  // Null-terminate the argument array
    return cmd;
}

int execute_cmd(cmdline *cmd) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        if (cmd->input_file) {
            freopen(cmd->input_file, "r", stdin);  // Redirect input from file
        }
        if (cmd->output_file) {
            freopen(cmd->output_file, "w", stdout);  // Redirect output to file
        }
        execvp(cmd->args[0], cmd->args);  // Execute the command
        perror("execvp");  // In case execvp fails
        exit(EXIT_FAILURE);  // Exit on failure
    } else if (pid > 0) {
        // Parent process
        if (cmd->background) {
            // Handle background job
            cmd->pid = pid;  // Store the process ID
            cmd->command = strdup(cmd->args[0]);  // Duplicate the command string
            jobs[job_count++] = cmd;  // Add job to the job list
            printf("[%d] %d\n", job_count, pid);  // Display background job info
        } else {
            waitpid(pid, NULL, 0);  // Wait for foreground process to finish
        }
    } else {
        perror("fork");  // Fork failed
        return -1;
    }
    return 0;
}

int count_pipes(char *cmd) {
    int count = 0;
    // Count the number of pipe symbols ('|') in the command
    for (int i = 0; i < strlen(cmd); i++) {
        if (cmd[i] == '|') {
            count++;
        }
    }
    return count;
}

void split_piped_cmds(char *cmd, char **piped_cmds) {
    // Split the input command by pipes and store in piped_cmds array
    char *token = strtok(cmd, "|");
    int i = 0;
    while (token != NULL) {
        piped_cmds[i++] = token;
        token = strtok(NULL, "|");
    }
    piped_cmds[i] = NULL;  // Null-terminate the array
}

int execute_multiple_pipes(char **piped_cmds, int num_pipes) {
    int pipefds[2 * num_pipes];  // File descriptors for pipes
    pid_t pid;
    int i;

    // Create all pipes
    for (i = 0; i < num_pipes; i++) {
        if (pipe(pipefds + i * 2) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);  // Exit if pipe creation fails
        }
    }

    int j = 0;
    while (piped_cmds[j] != NULL) {
        cmdline *cmd = parse_cmd(piped_cmds[j]);  // Parse each piped command
        pid = fork();
        if (pid == 0) {  // Child process
            // If not the first command, get input from the previous pipe
            if (j > 0) {
                dup2(pipefds[(j - 1) * 2], STDIN_FILENO);  // Input from previous pipe
            }

            // If not the last command, output to the next pipe
            if (piped_cmds[j + 1] != NULL) {
                dup2(pipefds[j * 2 + 1], STDOUT_FILENO);  // Output to next pipe
            }
            // Handle output redirection for the last command in the pipeline
            else if (cmd->output_file) {
                int fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                dup2(fd, STDOUT_FILENO);  // Redirect output to file
                close(fd);
            }

            // Close all pipe file descriptors
            for (i = 0; i < 2 * num_pipes; i++) {
                close(pipefds[i]);  // Closing unused file descriptors
            }

            // Execute the command
            execvp(cmd->args[0], cmd->args);  // Execute command
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        j++;
    }

    // Close all pipe file descriptors in the parent
    for (i = 0; i < 2 * num_pipes; i++) {
        close(pipefds[i]);
    }

    // Wait for all child processes
    for (i = 0; i < num_pipes + 1; i++) {
        wait(NULL);  // Wait for all children
    }

    return 0;
}

void list_jobs() {
    for (int i = 0; i < job_count; i++) {
        int status;
        // Check if the process is still running using waitpid with WNOHANG
        pid_t result = waitpid(jobs[i]->pid, &status, WNOHANG);
        
        if (result == 0) {
            // Process is still running, print it
            printf("[%d] %d %s\n", i + 1, jobs[i]->pid, jobs[i]->command);
        } else {
            // Process has completed, remove it from the job list
            free(jobs[i]->command);  // Free the memory for the command
            free(jobs[i]);  // Free the cmdline struct

            // Shift remaining jobs down
            for (int j = i; j < job_count - 1; j++) {
                jobs[j] = jobs[j + 1];  // Shift job list
            }
            job_count--;  // Decrease job count
            i--;  // Adjust the index since we shifted jobs
        }
    }
}

void wait_for_jobs() {
    // Wait for all background jobs to complete
    for (int i = 0; i < job_count; i++) {
        waitpid(jobs[i]->pid, NULL, 0);  // Wait for each job
    }
    job_count = 0;  // Reset the job count after waiting
}
