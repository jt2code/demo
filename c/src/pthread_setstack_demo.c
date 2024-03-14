#include "../include/utils.h"
#include <pthread.h>

void task(void *arg) { pause(); }

int main(void)
{
    int st;
    size_t ssize;
    pthread_t thread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_getstacksize(&attr, &ssize);
    printf("default stack size: %ld\n", ssize);

    st = pthread_attr_setstacksize(&attr, (1024 * 512)); /* 512 KB */
    if (st != 0) {
        FATAL_ERRNO(st, "pthread_attr_setstacksize");
    }

    pthread_attr_getstacksize(&attr, &ssize);
    printf("current stack size: %ld\n", ssize);

    pthread_create(&thread, &attr, (void *)task, NULL);
    puts("enter ctrl-c exit.");

    pthread_join(thread, NULL);

    return 0;
}