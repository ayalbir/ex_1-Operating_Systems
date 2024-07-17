# Part 3: Custom Shell Implementation

## Overview
This part involved implementing a custom shell in C named `myshell.c`. The shell supports basic functionalities such as command execution, input/output redirection, and background processing.

## Objectives
- Implement command execution.
- Support input/output redirection.
- Handle background processing with `&`.

## Implementation Details

### Basic Command Execution
The shell executes simple commands like `ls`, `pwd`, and `echo`. It creates a child process to run the command and waits for it to complete.

### Input/Output Redirection
The shell supports:
- `<`: Redirect input from a file
- `>`: Redirect output to a file (overwrite)
- `>>`: Redirect output to a file (append)

### Background Processing
The shell runs commands in the background using `&` at the end of the command. It returns to the prompt immediately without waiting for the command to complete.

### Example Usage
```bash
./myshell
```

### Example Commands
- Running a simple command: `ls`
- Redirecting input and output: `sort < input.txt > output.txt`
- Appending output: `echo "hello" >> output.txt`
- Running a command in the background: `sleep 10 &`

## Conclusion
This part demonstrates fundamental concepts in system programming and shell implementation. The `myshell.c` program effectively handles command execution, input/output redirection, and background processing, providing a basic yet functional custom shell.
