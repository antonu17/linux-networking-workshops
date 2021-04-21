#define _GNU_SOURCE
#include <sys/wait.h>
#include <sched.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

#define STACK_SIZE (1024 * 1024)
static char childStack[STACK_SIZE];

static int child_fn() {
    printf("new network namespace interfaces:\n");
    system("ip link");
    printf("\n");
    printf("\n");

    sleep(7200);
    return 0;
}

int main() {
    void *stackTop = childStack + STACK_SIZE;

    printf("Source PID: %jd\n", (intmax_t)getpid());
    printf("source network namespace interfaces:\n");
    system("ip link");
    printf("\n");
    printf("\n");

    pid_t childPid = clone(child_fn, stackTop, CLONE_NEWNET | SIGCHLD, NULL);
    if (childPid == -1)
        errExit("clone");

    printf("New PID: clone() returned %jd\n", (intmax_t) childPid);

    if (waitpid(childPid, NULL, 0) == -1)    /* Wait for child */
        errExit("waitpid");
    printf("child has terminated\n");

    exit(EXIT_SUCCESS);
}
