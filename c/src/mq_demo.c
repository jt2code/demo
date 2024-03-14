#include "../include/utils.h"
#include <mqueue.h>
#include <signal.h>
#include <string.h>

#define _DEMO_MQNAME "/mymq"

mqd_t getmqd(void);
void register_notify(mqd_t mqd);
void send_message(char *msg, size_t msg_len, unsigned int prio);
void receive_message(sigval_t val);
void terminate(int signo);

int main(int argc, char *argv[])
{
    mqd_t mqd = getmqd();

    if (argc > 1) {
        printf("[!] start sender\n");
        for (int i = 1; i < argc; i++)
            send_message(argv[i], strlen(argv[i]), 1);
        return 0;
    }

    printf("[!] start receiver\n");
    signal(SIGINT, terminate);
    signal(SIGTERM, terminate);

    struct mq_attr mqstat;
    ASSERT(mq_getattr(mqd, &mqstat) != -1, "mq_getattr");

    if (mqstat.mq_curmsgs > 0) {
        sigval_t sigval = {.sival_int = mqd};
        receive_message(sigval);
    } else {
        /* call mq_notify */
        register_notify(mqd);
    }

    for (;;)
        pause();
    return 0;
}

mqd_t getmqd(void)
{
    static mqd_t g_mq = -1;
    struct mq_attr attr = {.mq_maxmsg = 10, .mq_msgsize = 128};
    if (g_mq == -1) {
        g_mq = mq_open(_DEMO_MQNAME, O_CREAT | O_NONBLOCK | O_RDWR, S_IRWXU,
                       &attr);
        ASSERT(g_mq != -1, "mq_open");
    }
    return g_mq;
}

void register_notify(mqd_t mqd)
{
    static struct sigevent se;
    if (se.sigev_notify == 0) {
        se.sigev_notify = SIGEV_THREAD;
        se.sigev_value.sival_int = mqd;
        se.sigev_notify_function = &receive_message;
    }
    ASSERT(mq_notify(mqd, &se) != -1, "mq_notify");
    printf("[!] waiting for notification\n");
}

void receive_message(sigval_t val)
{
    int rn;
    char *buf;
    mqd_t mqd = val.sival_int;
    struct mq_attr mqstat;
    mq_getattr(mqd, &mqstat);
    buf = malloc(mqstat.mq_msgsize + 1);
    while ((rn = mq_receive(mqd, buf, mqstat.mq_msgsize, NULL)) != -1) {
        buf[rn] = '\0';
        printf("[+] receive: %s\n", buf);
    }
    if (errno == EAGAIN) {
        printf("[!] register for notification\n");
        register_notify(mqd);
    } else {
        FATAL("receive_message");
    }
}

void send_message(char *msg, size_t msg_len, unsigned int prio)
{
    int wn;
    mqd_t mqd = getmqd();
    do {
        wn = mq_send(mqd, msg, msg_len, prio);
        if (wn == -1) {
            if (errno == EAGAIN) {
                sleep(1);
                continue;
            } else {
                FATAL("mq_send");
            }
        }
    } while (wn == -1);
}

void terminate(int signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        ASSERT(mq_unlink(_DEMO_MQNAME) != -1, "mq_unlink");
        printf("\n[-] removed mq: %s\n", _DEMO_MQNAME);
    }
    exit(EXIT_SUCCESS);
}