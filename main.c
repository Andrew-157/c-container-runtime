#define _GNU_SOURCE
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

int child_function(void *arg){
    printf("Inside new namespace!\n");
    system("/bin/bash"); // Launch a shell in the new namespace
    return 0;
}

int main() {

    //printf("Main process ID: %d\n", getpid());

    pid_t child_pid = clone(child_function, child_stack + STACK_SIZE,
                      CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS | SIGCHLD, NULL);
    // CLONE_NEWPID - Creates a new PID namespace for the child process.
    // CLONE_NEWUTS - Creates a new UTS namespace, allowing the child to have a unique hostname.
    // CLONE_NEWNS  - Creates a new mount namespace for an isolated filesystem view.


    if (child_pid == -1) {
        perror("clone");
        exit(EXIT_FAILURE);
    }

    printf("Child process ID: %d\n", child_pid);
    waitpid(child_pid, NULL, 0);
    return 0;
}
