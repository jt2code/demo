#include "../include/utils.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define _DEMO_SHARED_MEM_NAME "/myshared"

int getshmfd()
{
    int shmfd;
    shmfd = shm_open(_DEMO_SHARED_MEM_NAME, O_CREAT | O_RDWR, S_IRWXU);
    ASSERT(shmfd != -1, "shm_open");
    return shmfd;
}

int main(void)
{
    char buf[16];
    int pid, shmfd, n;

    pid = fork();
    ASSERT(pid != -1, "fork");

    if (pid == 0) {
        shmfd = getshmfd(); /* child shared memory fd */
        const char str[] = "helloworld";
        n = write(shmfd, str, sizeof(str));
        ASSERT(n != -1, "write");
        printf("[CHILD] write %d bytes\n", n);
        _exit(0);
    }

    shmfd = getshmfd(); /* parent shared memory fd */
    usleep(1000);       /* wait for child writing */
    n = read(shmfd, buf, sizeof(buf));
    ASSERT(n != -1, "read");
    printf("[PARENT] read %d bytes: %s\n", n, buf);
    wait(NULL);
    shm_unlink(_DEMO_SHARED_MEM_NAME);
    return 0;
}