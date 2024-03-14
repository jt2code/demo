#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define _DEMO_MAX_TN 5

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void task(void)
{
    int tid = (long)pthread_self() % 1000;
    pthread_mutex_lock(&mutex);
    printf("[task] %3d readying\n", tid);
    pthread_cond_wait(&cond, &mutex);
    printf("[task] %3d running\n", tid);
    /* do something */
    pthread_mutex_unlock(&mutex);
}

void control(void)
{
    printf("[main] execute all tasks after 5 seconds\n");
    for (int i = 5; i > 0; i--) {
        printf("[main] %d ...\n", i);
        sleep(1);
    }
    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

int main(void)
{
    pthread_t ths[_DEMO_MAX_TN];
    for (int i = 0; i < _DEMO_MAX_TN; i++)
        pthread_create(&ths[i], NULL, (void *)task, NULL);

    usleep(100000);
    control();
    /* waitting for threads */
    for (int i = 0; i < _DEMO_MAX_TN; i++)
        pthread_join(ths[i], NULL);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    return 0;
}