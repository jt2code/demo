#include "../include/utils.h"
#include <signal.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define _DEMO_MSGQ_BYTES 1024

struct msgbuf {
    long mtype;     /* message type, must be > 0 */
    char mtext[16]; /* message data */
};

int mqid;
void send_msg(int mqid, long msgtype, char *data);
void recv_msg(int mqid, long msgtype);

void sighandler(int sig)
{
    printf("removed mqid %d\n", mqid);
    msgctl(mqid, IPC_RMID, NULL);
    exit(0);
}

int main(int argc, char *argv[])
{
    mqid = msgget(ftok("/tmp", 1), IPC_CREAT | IPC_EXCL);
    if (mqid != -1) {
        struct msqid_ds dsbuf = {
            .msg_qbytes = _DEMO_MSGQ_BYTES,
            .msg_perm = {.mode = 0666},
        };
        ASSERT(msgctl(mqid, IPC_SET, &dsbuf) != -1, "msgctl");
    } else if (errno == EEXIST) {
        mqid = msgget(ftok("/tmp", 1), 0);
        ASSERT(mqid != -1, "msgget");
    } else {
        FATAL("msgget");
    }

    if (argc > 1) {
        if (strncmp(argv[1], "p", 1) == 0 && argc > 3) {
            send_msg(mqid, atol(argv[2]), argv[3]);
        } else if (strncmp(argv[1], "c", 1) == 0 && argc > 2) {
            signal(SIGINT, sighandler);
            recv_msg(mqid, atol(argv[2]));
        }
    } else {
        /* print help */
        printf("(producer) %s p msgtype msg\n", argv[0]);
        printf("(consumer) %s c msgtype\n", argv[0]);
    }

    return 0;
}

void send_msg(int mqid, long msgtype, char *data)
{
    if (msgtype < 1)
        return;
    struct msgbuf buf;
    buf.mtype = msgtype;
    strncpy(buf.mtext, data, sizeof(buf.mtext));
    msgsnd(mqid, (void *)&buf, sizeof(buf), 0);
    printf("send msg, type: %ld\n", buf.mtype);
}

void recv_msg(int mqid, long msgtype)
{
    ssize_t sz;
    struct msgbuf buf;
    printf("receiving msgtype: %ld\n", msgtype);
    while (msgtype > 0) {
        sz = msgrcv(mqid, (void *)&buf, sizeof(buf), msgtype, 0);
        if (sz > 0) {
            buf.mtext[sz] = '\0';
            printf("%s\n", buf.mtext);
        }
    }
}