#include "../include/utils.h"
#include <pthread.h>
#include <signal.h>
#include <string.h>

void sig_handler(int sig)
{
    printf("[!] sig_handler signal: <%s>\n", strsignal(sig));
}

void sigaction_handler(int sig, siginfo_t *info, void *ucontent)
{
    printf("[!] sigaction_handler signal: <%s>, sender pid: %d\n",
           strsignal(sig), info->si_pid);
}

void routine(void *ptr)
{
    pid_t pid = getpid();
    printf("[+] send SIGINT to self\n");
    kill(pid, SIGINT);
    sleep(1);
    printf("[+] send SIGTERM to self\n");
    kill(pid, SIGTERM);
}

int main(void)
{
    /* sigaction function example */
    struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = sigaction_handler;
    sigaction(SIGTERM, &act, NULL);

    /* signal function example */
    signal(SIGINT, sig_handler);

    /* send signal to self */
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&thread, &attr, (void *)routine, NULL);

    pause();
    pause();
    return 0;
}