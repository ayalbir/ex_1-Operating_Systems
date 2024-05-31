//214104226 Ayal Birenstock
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 100

char *history_list[MAX_LINE];
int history_count = 0;
char **original_env;

void restore_env() {
    for (int i = 0; original_env[i] != NULL; i++) {
        putenv(original_env[i]);
    }
}

void cd(char *path) {
    if (chdir(path) != 0) {
        perror("cd failed");
        exit(1);
    }
}

void pwd() {
    //pwd using getcwd
    char cwd[MAX_LINE];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("pwd failed");
        exit(1);
    } else {
        printf("%s\n", cwd);
    }
}

void history() {
    //"history prints in FIFO and includes the history command itself" 
    for (int i = 0; i < history_count; ++i) {
        printf("%d %s\n", i, history_list[i]);
    }
}

void add_to_history(char *command) {
    history_list[history_count] = strdup(command);
    history_count++;
}

void my_cat(char *file) {
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0) {
        char *args[] = {"/bin/cat", file, NULL};
        if (execv(args[0], args) < 0) {
            perror("execv failed");
            exit(1);
        }
    } else {
        if (wait(NULL) < 0) {
            perror("wait failed");
            exit(1);
        }
    }
}

void my_exit() {
    //exit. not much to change...
    restore_env();
    exit(0);
}

void my_sleep(char *seconds) {
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0) {
        char *args[] = {"/bin/sleep", seconds, NULL};
        if (execv(args[0], args) < 0) {
            perror("execv failed");
            exit(1);
        }
    } else {
        if (wait(NULL) < 0) {
            perror("wait failed");
            exit(1);
        }
    }
}

void my_ls() {
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0) {
        char *args[] = {"/bin/ls", NULL};
        if (execv(args[0], args) < 0) {
            perror("execv failed");
            exit(1);
        }
    } else {
        if (wait(NULL) < 0) {
            perror("wait failed");
            exit(1);
        }
    }
}

void execute_command(char *command, char *argv[]) {
    pid_t pid;
    if ((pid = fork()) < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0) {
        // In child process, modify the environment variables as needed
        if (strncmp(command, "./", 2) == 0) {
            // If the command starts with "./", try to execute it directly
            if (access(command, X_OK) == 0) {
                if (execv(command, argv) < 0) {
                    perror("execv failed");
                    exit(1);
                }
                return;
            }
        } else {
            // Otherwise, search for the command in the PATH
            char *path = getenv("PATH");
            char *path_copy = strdup(path);
            char *dir = strtok(path_copy, ":");

            while (dir != NULL) {
                char possible_path[MAX_LINE];
                sprintf(possible_path, "%s/%s", dir, command);

                if (access(possible_path, X_OK) == 0) {
                    argv[0] = possible_path;
                    if (execv(possible_path, argv) < 0) {
                        perror("execv failed");
                        exit(1);
                    }
                    return;
                }

                dir = strtok(NULL, ":");
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
    add_to_history(command);
}


void decide_command(char *command, char *argv[]) {
    if (command == NULL) {
        return;
    }
    if (strcmp(command, "cd") == 0) {
        cd(argv[1]);
    } else if (strcmp(command, "pwd") == 0) {
        add_to_history(command);
        pwd();
    } else if (strcmp(command, "history") == 0) {
        add_to_history(command);
        history();
    } else if (strcmp(command, "cat") == 0) {
        add_to_history(command);
        my_cat(argv[1]);
    } else if (strcmp(command, "exit") == 0) {
        add_to_history(command);
        my_exit();
    } else if (strcmp(command, "sleep") == 0) {
        add_to_history(command);
        my_sleep((argv[1]));
    } else if (strcmp(command, "ls") == 0) {
        add_to_history(command);
        my_ls();
    } else {
        execute_command(command, argv);
    }

}



int main(int argc, char *argv[], char *envp[]) {
    original_env = envp;
    for (int i = 0; i < argc; ++i) {
        char var_name[MAX_LINE];
        //variables for the arguments
        sprintf(var_name, "ARG%d", i);
        //add them to the environment variables
        setenv(var_name, argv[i], 1);
    }

    //do the first command received from the user
    if (argv[1] != NULL) {
        //ditch the first argument, since it is the name of the program
        argv++;
        decide_command(argv[0], argv);
    }

    while (1) {
        //continuously read the input from the user and execute the command
        printf("$ ");
        fflush(stdout);
        char input[MAX_LINE];
        fgets(input, MAX_LINE, stdin);
        input[strlen(input) - 1] = '\0';

        char *command_args[MAX_LINE];
        char *command = strtok(input, " ");
        int i = 0;
        while (command != NULL) {
            command_args[i] = command;
            command = strtok(NULL, " ");
            i++;
        }
        command_args[i] = NULL; // Null-terminate the array

        decide_command(command_args[0], command_args);
    }
}