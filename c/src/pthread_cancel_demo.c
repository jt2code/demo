#include "../include/utils.h"
#include <pthread.h>

int __thread do_cleanup_pop = 1;

void cleanup(void *arg)
{
    do_cleanup_pop = 0;
    printf("[-] run thread cleanup\n");
}

/**
 * default state and type
 * pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
 * pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
 */
void task(void)
{
    pthread_cleanup_push(cleanup, NULL);
    for (;;) {
        printf("[+] running task\n");
        sleep(1);
        // pthread_testcancel();
    }
    pthread_cleanup_pop(do_cleanup_pop);
}

int main(void)
{
    pthread_t thread;
    pthread_create(&thread, NULL, (void *)task, NULL);

    sleep(3);
    pthread_cancel(thread);

    pthread_join(thread, NULL);
    return 0;
}