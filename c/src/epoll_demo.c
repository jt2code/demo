#define _DEMO_UTILS_NET
#include "../include/utils.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#define _DEMO_LISTEN_HOST "127.0.0.1"
#define _DEMO_LISTEN_PORT 8899
#define _DEMO_MAX_EVENTS 10

typedef struct rd_args {
    int fd, epfd;
    SA_IN *addr;
} rd_args_t;

int setnonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    if (flags != -1) {
        return fcntl(fd, F_SETFL, flags | SOCK_NONBLOCK);
    }
    return -1;
}

void print_sockaddr(SA_IN *addr, char *s)
{
    char buf[20];
    sprintf_sockaddr(addr, buf, sizeof(buf));
    printf("<%s> %s\n", buf, s);
}

int do_read(rd_args_t *args)
{
    int rn;
    char buf[128];
    rn = recv(args->fd, buf, sizeof(buf), 0);
    if (rn < 1) {
        epoll_ctl(args->epfd, EPOLL_CTL_DEL, args->fd, NULL);
        close(args->fd);
        print_sockaddr(args->addr, "closed");
        /* free args mem */
        free(args);
    } else {
        /* print(buf, rn, 1, stdout); */
        ASSERT(send(args->fd, buf, rn, 0) != -1, "send");
    }
    return 0;
}

int main(int argc, char *argv[])
{
    SA_IN listen_addr, conn_addr;
    int sockfd, connfd, epfd, nfds;
    struct epoll_event event, event_list[_DEMO_MAX_EVENTS];
    socklen_t conn_addr_len = sizeof(conn_addr);
    const int reuse_addr = 1;

    ASSERT(init_sockaddr_in(&listen_addr, _DEMO_LISTEN_HOST,
                            _DEMO_LISTEN_PORT) != -1,
           "init sockaddr");

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT(sockfd != -1, "socket");

    ASSERT(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
                      sizeof(reuse_addr)) != -1,
           "setsockopt");

    ASSERT(bind(sockfd, (SA *)&listen_addr, sizeof(listen_addr)) != -1, "bind");
    ASSERT(listen(sockfd, SOMAXCONN) != -1, "listen");

    printf("Listen TCP %s:%d\n", _DEMO_LISTEN_HOST, _DEMO_LISTEN_PORT);

    epfd = epoll_create(1);
    ASSERT(epfd != -1, "epoll_create");

    event.events = EPOLLIN;
    event.data.fd = sockfd;
    ASSERT(epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event) != -1, "epoll_ctl");

    for (;;) {
        nfds = epoll_wait(epfd, event_list, _DEMO_MAX_EVENTS, -1);
        ASSERT(nfds != -1, "epoll_wait");

        for (int i = 0; i < nfds; i++) {
            if (event_list[i].data.fd == sockfd) {
                /* is listen socket fd */
                connfd = accept(sockfd, (SA *)&conn_addr, &conn_addr_len);
                ASSERT(connfd != -1, "accept");
                /* set nonblock */
                setnonblocking(connfd);

                /* set do_read function args */
                rd_args_t *ptr = malloc(sizeof(rd_args_t));
                ptr->fd = connfd;
                ptr->epfd = epfd;
                ptr->addr = &conn_addr;

                event.data.ptr = ptr;
                event.events = EPOLLIN;
                ASSERT(epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &event) != -1,
                       "epoll_ctl");
                /* print client info */
                print_sockaddr(&conn_addr, "established");
            } else {
                if (event_list[i].events & EPOLLIN) {
                    do_read(event_list[i].data.ptr);
                }
            }
        }
    }

    return 0;
}