#include "../include/utils.h"
#include <fcntl.h>

int main(void)
{
    pid_t pid;
    int pipe_fds[2];
    char buf[128];

    ASSERT(pipe(pipe_fds) != -1, "pipe");

    if ((pid = fork()) == 0) {
        /* child */
        printf("[CHILD] exec `date` command\n");
        /* redirection stdout */
        ASSERT(dup2(pipe_fds[1], 1) != -1, "dup2");

        close(pipe_fds[0]);
        /* execlp(cmd, argv0, argv1, ...) */
        if (execlp("date", "date", "+%F %T", NULL) == -1)
            FATAL("exec");
    } else if (pid > 0) {
        /* parent */
        close(pipe_fds[1]);
        int rn = read(pipe_fds[0], buf, sizeof(buf));
        if (rn > 0) {
            buf[rn] = 0;
            printf("[PARENT] child output: %s", buf);
        }
    } else {
        FATAL("fork");
    }

    return 0;
}