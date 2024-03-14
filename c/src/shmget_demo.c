#include "../include/utils.h"
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define _DEMO_SHMSZ 64

int proc_1(void);
int proc_2(void);

key_t shmkey;

int main(void)
{
    shmkey = ftok("/tmp", 1);
    ASSERT(shmkey != -1, "ftok");

    if (fork() == 0) {
        proc_1();
        return 0;
    }

    if (fork() == 0) {
        proc_2();
        return 0;
    }

    while (wait(NULL) != -1) {
    }

    return 0;
}

int getshmid(size_t shmsz)
{
    int shmid = shmget(shmkey, shmsz, IPC_CREAT | IPC_EXCL);
    if (shmid != -1) {
        /* update permission */
        struct shmid_ds buf = {
            .shm_perm = {.mode = 0660, .uid = getuid(), .gid = getgid()},
        };
        ASSERT(shmctl(shmid, IPC_SET, &buf) != -1, "shmctl");
    } else if (errno == EEXIST) {
        /* printf("warning: key is exists\n"); */
        shmid = shmget(shmkey, shmsz, 0);
    } else {
        FATAL("shmget");
    }
    return shmid;
}

int proc_1(void)
{
    int shmid = getshmid(_DEMO_SHMSZ);
    void *shaddr = shmat(shmid, NULL, 0);
    ASSERT(shaddr != NULL, "shmat");
    strcpy(shaddr, "helloworld");
    printf("[PROC_1] write shmid (%d)\n", shmid);
    shmdt(shaddr);
    return 0;
}

int proc_2(void)
{
    int shmid = getshmid(_DEMO_SHMSZ);
    void *shaddr = shmat(shmid, NULL, SHM_RDONLY);
    ASSERT(shaddr != NULL, "shmat");
    /* wait brother writing */
    usleep(1000);
    char *data = shaddr;
    printf("[PROC_2] read shm data: %s\n", data);

    shmdt(shaddr);
    /* remove shared memory */
    if (shmctl(shmid, IPC_RMID, NULL) == 0)
        printf("[PROC_2] removed shmid (%d)\n", shmid);

    return 0;
}