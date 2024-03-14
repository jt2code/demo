#include "../include/utils.h"
#include <pthread.h>

#define _DEMO_MAX_TN 10

/* thread private variables */
static __thread int thread_num;
/* pthread data key */
pthread_key_t thread_key;
pthread_once_t once = PTHREAD_ONCE_INIT;

void task(void)
{
    thread_num++;
    printf("[+] task <%ld> addr: %p value: %d\n", pthread_self(), &thread_num,
           thread_num);

    int *buf = malloc(sizeof(int));
    ASSERT(buf != NULL, "malloc");
    *buf = pthread_self() % 1000;
    pthread_setspecific(thread_key, buf);
}

void destroy(void *buf)
{
    sleep(1);
    printf("[-] dest <%ld> addr: %p value: %d\n", pthread_self(), buf,
           *(int *)buf);
    free(buf);
}

void init_key(void)
{
    int st = pthread_key_create(&thread_key, (void *)destroy);
    if (st != 0) {
        FATAL_ERRNO(st, "pthread_key_create");
    }
}

int main(void)
{
    pthread_t thread[_DEMO_MAX_TN];

    pthread_once(&once, (void *)init_key);

    for (int i = 0; i < _DEMO_MAX_TN; i++)
        pthread_create(&thread[i], NULL, (void *)task, NULL);

    for (int i = 0; i < _DEMO_MAX_TN; i++)
        pthread_join(thread[i], NULL);

    pthread_key_delete(thread_key);
    return 0;
}