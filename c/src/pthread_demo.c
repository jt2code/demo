#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define _DEMO_MAX_TN 5

void task(unsigned long num)
{
    printf("task <%ld> running\n", num);
    sleep(1);
    printf("task <%ld> done\n", num);
}

int main(void)
{
    unsigned long i;
    pthread_t ths[_DEMO_MAX_TN];

    for (i = 0; i < _DEMO_MAX_TN; i++)
        pthread_create(&ths[i], NULL, (void *)task, (void *)i);
    /* waitting for threads */
    for (i = 0; i < _DEMO_MAX_TN; i++)
        pthread_join(ths[i], NULL);

    return 0;
}