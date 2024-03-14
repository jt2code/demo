#include <pthread.h>
#include <stdio.h>

#define _DEMO_MAX_TN 5

typedef struct {
    int n;
    pthread_rwlock_t *l;
} taskargs_t;

void task_read(taskargs_t *args)
{
    pthread_rwlock_rdlock(args->l);
    printf("\033[33mR Lock   <%d>\033[0m\n", args->n);
    /* do somthing */
    printf("\033[33mR UnLock <%d>\033[0m\n", args->n);
    pthread_rwlock_unlock(args->l);
}

void task_write(taskargs_t *args)
{
    for (int i = 0; i < 3; i++) {
        pthread_rwlock_wrlock(args->l);
        printf("\033[31mW Lock   <%d>\033[0m\n", args->n);
        /* do somthing */
        printf("\033[31mW Unlock <%d>\033[0m\n", args->n);
        pthread_rwlock_unlock(args->l);
    }
}

int main(void)
{
    int i;
    pthread_t ths[_DEMO_MAX_TN];
    taskargs_t args[_DEMO_MAX_TN];
    pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

    for (i = 0; i < _DEMO_MAX_TN; i++) {
        args[i].n = i;
        args[i].l = &rwlock;
        if (i == 0) {
            /* create write lock task */
            pthread_create(&ths[i], NULL, (void *)task_write, (void *)&args[i]);
        } else {
            /* create read lock task */
            pthread_create(&ths[i], NULL, (void *)task_read, (void *)&args[i]);
        }
    }
    /* waitting for threads */
    for (i = 0; i < _DEMO_MAX_TN; i++)
        pthread_join(ths[i], NULL);

    pthread_rwlock_destroy(&rwlock);
    return 0;
}