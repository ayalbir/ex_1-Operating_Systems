// 214104226 Ayal Birenstock
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#pragma GCC optimize("O2")

#define MAX_LINE 100
#define MAX_HISTORY 50

char *history_list[MAX_HISTORY];
int history_count = 0;
char **original_env;
char *search_paths[MAX_LINE];
int search_path_count = 0;

void restore_env() {
    for (int i = 0; original_env[i] != NULL; i++) {
        putenv(strdup(original_env[i]));
    }
}

void cd(char *path) {
    if (chdir(path) != 0) {
        perror("cd failed");
    }
}

void pwd() {
    char cwd[MAX_LINE];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("pwd failed");
    } else {
        printf("%s\n", cwd);
    }
}

void history() {
    for (int i = 0; i < history_count; ++i) {
        printf("%s\n", history_list[i]);
    }
}

void add_to_history(char *command, char *argv[]) {
    char full_command[MAX_LINE] = "";
    strcat(full_command, command);
    for (int i = 1; argv[i] != NULL; ++i) {
        strcat(full_command, " ");
        strcat(full_command, argv[i]);
    }

    if (history_count < MAX_HISTORY) {
        history_list[history_count] = strdup(full_command);
        history_count++;
    } else {
        free(history_list[0]);
        for (int i = 1; i < MAX_HISTORY; ++i) {
            history_list[i - 1] = history_list[i];
        }
        history_list[MAX_HISTORY - 1] = strdup(full_command);
    }
}
void my_exit() {
    //not much to do here...
    restore_env();
    exit(0);
}

void execute_command(char *command, char *argv[], char **envp) {
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0) {
        // Check the current directory for the command
        if (access(command, X_OK) == 0) {
            // Execute the command
            if (execve(command, argv, envp) < 0) {
                fprintf(stderr, "Execution of %s failed\n", command);
                perror("execv failed");
                exit(1);
            }
            return;  //will get here only if execv fails
        }

        // Search for the command in the given paths and environment
        for (int i = 0; i < search_path_count; ++i) {
            char possible_path[MAX_LINE];
            sprintf(possible_path, "%s/%s", search_paths[i], command);

            if (access(possible_path, X_OK) == 0) {
                // Execute the command
                if (execve(possible_path, argv, envp) < 0) {
                    fprintf(stderr, "Execution of %s failed\n", command);
                    perror("execv failed");
                    exit(1);
                }
                return;  //same
            }
        }

        printf("Command not found\n");
        exit(0);
    } else {
        if (wait(NULL) < 0) {
            perror("wait failed");
            exit(1);
        }
    }
    add_to_history(command, argv);
}

void decide_command(char *command, char *argv[], char **envp) {
    if (command == NULL) {
        return;
    }
    if (strcmp(command, "cd") == 0) {
        cd(argv[1]);
    } else if (strcmp(command, "pwd") == 0) {
        add_to_history(command, argv);
        pwd();
    } else if (strcmp(command, "history") == 0) {
        add_to_history(command, argv);
        history();
    } else if (strcmp(command, "exit") == 0) {
        add_to_history(command, argv);
        my_exit();
    }
    else {
        execute_command(command, argv, envp);
    }
}

int main(int argc, char *argv[], char *envp[]) {
    original_env = envp;

    for (int i = 1; i < argc; ++i) {
        search_paths[search_path_count++] = strdup(argv[i]);
    }

    char *path_env = getenv("PATH");
    if (path_env != NULL) {
        char *path = strtok(path_env, ":");
        while (path != NULL) {
            search_paths[search_path_count++] = strdup(path);
            path = strtok(NULL, ":");
        }
    }

    while (1) {
        printf("$ ");
        fflush(stdout);
        char input[MAX_LINE];
        if (fgets(input, MAX_LINE, stdin) == NULL) {
            perror("fgets failed");
            continue;
        }
        input[strlen(input) - 1] = '\0';

        char *command_args[MAX_LINE];
        char *command = strtok(input, " ");
        int i = 0;
        while (command != NULL) {
            command_args[i] = command;
            command = strtok(NULL, " ");
            i++;
        }
        command_args[i] = NULL;

        decide_command(command_args[0], command_args, envp);
    }

    //even this is not needed, since we are exiting only using "exit"... still.
    for (int i = 0; i < search_path_count; ++i) {
        free(search_paths[i]);
    }
    return 0;
}
