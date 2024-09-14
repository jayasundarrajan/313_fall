/****************
LE2: Introduction to Unnamed Pipes
****************/
#include <unistd.h> // pipe, fork, dup2, execvp, close
#include <sys/wait.h> // wait
#include <iostream>
using namespace std;

int main () {
    // lists all the files in the root directory in the long format
    char* cmd1[] = {(char*) "ls", (char*) "-al", (char*) "/", nullptr};
    // translates all input from lowercase to uppercase
    char* cmd2[] = {(char*) "tr", (char*) "a-z", (char*) "A-Z", nullptr};

    // TODO: add functionality
    // Create pipe

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Create child to run first command

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        // In child, redirect output to write end of pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        // Close the read end of the pipe on the child side.
        // In child, execute the command
        execvp(cmd1[0], cmd1);
        perror("execvp");
        return 1;
    }
    // In child, redirect output to write end of pipe
    // Close the read end of the pipe on the child side.
    // In child, execute the command

    // Create another child to run second command
    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        return 1;
    }
    if (pid2 == 0) {
        // In child, redirect input to the read end of the pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        // Close the write end of the pipe on the child side.
        // In child, execute the second command.
        execvp(cmd2[0], cmd2);
        perror("execvp");
        return 1;
    }
    // In child, redirect input to the read end of the pipe
    // Close the write end of the pipe on the child side.
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid, nullptr, 0);
    waitpid(pid2, nullptr, 0);
    

    // Execute the second command.

    // Reset the input and output file descriptors of the parent.
    return 0;
}
