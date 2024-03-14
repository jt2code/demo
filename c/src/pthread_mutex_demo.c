#include <pthread.h>
#include <stdio.h>

#define _DEMO_MAX_TN 5
#define _DEMO_USE_MUTEX 1
#define _DEMO_MAX_GLOBAL_N 10000

int global_number = 0;

void task(pthread_mutex_t *mutex)
{
#if _DEMO_USE_MUTEX
    pthread_mutex_lock(mutex);
#endif
    for (int i = 0; i < _DEMO_MAX_GLOBAL_N; i++)
        global_number++;
#if _DEMO_USE_MUTEX
    pthread_mutex_unlock(mutex);
#endif
}

int main(void)
{
    int i;
    pthread_t ths[_DEMO_MAX_TN];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    for (i = 0; i < _DEMO_MAX_TN; i++)
        pthread_create(&ths[i], NULL, (void *)task, (void *)&mutex);
    /* waitting for threads */
    for (i = 0; i < _DEMO_MAX_TN; i++)
        pthread_join(ths[i], NULL);

    pthread_mutex_destroy(&mutex);
    printf("Global number: %d\n", global_number);
    return 0;
}