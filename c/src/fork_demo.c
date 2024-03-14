#include "../include/utils.h"
#include <wait.h>

int main(void)
{
    pid_t pid;
    int child_stat;

    if ((pid = fork()) == 0) {
        /* child */
        printf("[CHILD] my pid: <%d>\n", getpid());
        _exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        /* parent */
        printf("[PARENT] my pid: <%d>\n", getpid());
        wait(&child_stat);
        printf("[PARENT] my child <%d> exit state: %d\n", pid,
               WEXITSTATUS(child_stat));
    } else {
        /* error */
        FATAL("fork");
    }

    return 0;
}