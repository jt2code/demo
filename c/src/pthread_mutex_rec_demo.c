#include "../include/utils.h"
#include <pthread.h>
#include <stdio.h>
#define _DEMO_MAX_TN 2
#define _DEMO_MAX_GLOBAL_N 10

int global_number = 0;

struct taskargs {
    int num;
    pthread_mutex_t *mtx;
};

void task(struct taskargs *args)
{
    printf("task <%d> number: %d\n", args->num, global_number);
    pthread_mutex_lock(args->mtx);
    if (global_number < _DEMO_MAX_GLOBAL_N) {
        global_number++;
        task(args);
    }
    pthread_mutex_unlock(args->mtx);
}

int main(void)
{
    int i, st;
    pthread_t ths[_DEMO_MAX_TN];
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutex_attr;
    struct taskargs args[_DEMO_MAX_TN];

    pthread_mutexattr_init(&mutex_attr);
    st = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    if (st > 0) {
        FATAL_ERRNO(st, "set recursive type");
    }
    pthread_mutex_init(&mutex, &mutex_attr);

    for (i = 0; i < _DEMO_MAX_TN; i++) {
        args[i].num = i;
        args[i].mtx = &mutex;
        pthread_create(&ths[i], NULL, (void *)task, (void *)&args[i]);
    }
    /* waitting for threads */
    for (i = 0; i < _DEMO_MAX_TN; i++)
        pthread_join(ths[i], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mutex_attr);
    return 0;
}