#include "../include/utils.h"
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define USE_THREAD_NOTIFY 1

int times = 3;

void sig_handler(int signo)
{
    times--;
    printf("[sig_handler] signal: <%s>, times(%d)\n", strsignal(signo), times);
}

void sig_handler_thread(sigval_t st)
{
    times--;
    printf("[sig_handler_thread] signal: <%s>, times(%d)\n",
           strsignal(st.sival_int), times);
}

int main(void)
{
    timer_t timerid;
    sigset_t sigset;
    struct sigevent sigevent;
    struct itimerspec iti;

    sigevent.sigev_signo = SIGUSR1;
#if USE_THREAD_NOTIFY
    pthread_attr_t thread_attr;
    sigevent.sigev_notify = SIGEV_THREAD;
    sigevent.sigev_value.sival_int = sigevent.sigev_signo;
    sigevent._sigev_un._sigev_thread._attribute = &thread_attr;
    sigevent._sigev_un._sigev_thread._function = sig_handler_thread;
#else
    sigevent.sigev_notify = SIGEV_SIGNAL;
#endif

    sigemptyset(&sigset);
    sigaddset(&sigset, sigevent.sigev_signo);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

#if USE_THREAD_NOTIFY
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
#else
    signal(sigevent.sigev_signo, sig_handler);
#endif

    /* create timer */
    ASSERT(timer_create(CLOCK_REALTIME, &sigevent, &timerid) != -1,
           "timer_create");

    bzero(&iti, sizeof(iti));
    iti.it_value.tv_nsec = 1; /* nanoseconds */
    iti.it_interval.tv_sec = 1;
    /* start timer */
    ASSERT(timer_settime(timerid, 0, &iti, NULL) != -1, "timer_settime");
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);

    while (times) {
        if (times == 1) {
            /* delete timer */
            ASSERT(timer_delete(timerid) != -1, "timer_delete");
            break;
        }
#if !USE_THREAD_NOTIFY
        pause();
#endif
    }
    return 0;
}