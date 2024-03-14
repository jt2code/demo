#include <arpa/inet.h>
#include <netinet/ip.h>
#include <string.h>
#include <sys/socket.h>

#define _DEMO_UTILS_NET
#include "../include/utils.h"

#define _DEMO_LISTEN_UDP_ADDR "0.0.0.0"
#define _DEMO_LISTEN_UDP_PORT 8899
#define _DEMO_MULTICAST_GROUP "224.0.0.69"
#define _DEMO_BUFSIZE 32

int multicast_receiver(void)
{
    int fd, rn;
    char recvbuf[_DEMO_BUFSIZE], peername[20];
    SA_IN listen_addr, remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);
    const int reuse_addr = 1;

    ASSERT((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1, "socket");

    ASSERT(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
                      sizeof(reuse_addr)) != -1,
           "setsockopt");

    ASSERT(init_sockaddr_in(&listen_addr, _DEMO_LISTEN_UDP_ADDR,
                            _DEMO_LISTEN_UDP_PORT) != -1,
           "init sockaddr");
    ASSERT(bind(fd, (SA *)&listen_addr, sizeof(listen_addr)) != -1, "bind");

    struct ip_mreq merq;
    merq.imr_interface.s_addr = INADDR_ANY;
    inet_pton(AF_INET, _DEMO_MULTICAST_GROUP, &merq.imr_multiaddr.s_addr);
    ASSERT(setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &merq, sizeof(merq)) !=
               -1,
           "setsockopt");

    printf("Listen UDP %s:%d\n", _DEMO_LISTEN_UDP_ADDR, _DEMO_LISTEN_UDP_PORT);

    for (;;) {
        rn = recvfrom(fd, recvbuf, sizeof(recvbuf) - 1, 0, (SA *)&remote_addr,
                      &remote_addr_len);
        if (rn == -1)
            break;
        recvbuf[rn] = '\0';

        sprintf_sockaddr(&remote_addr, peername, sizeof(peername));
        printf("data(%d): %s, from %s\n", rn, recvbuf, peername);
        ASSERT(sendto(fd, recvbuf, rn, 0, (SA *)&remote_addr,
                      remote_addr_len) != -1,
               "sendto");
    }
    return 0;
}

int multicast_sender(void)
{
    int fd, rn;
    SA_IN addr;
    char recvbuf[_DEMO_BUFSIZE];
    const char data[] = "helloworld";

    ASSERT((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1, "socket");
    ASSERT(init_sockaddr_in(&addr, _DEMO_MULTICAST_GROUP,
                            _DEMO_LISTEN_UDP_PORT) != -1,
           "init sockaddr");
    ASSERT(sendto(fd, data, sizeof(data) - 1, 0, (SA *)&addr, sizeof(addr)) !=
               -1,
           "sendto");

    struct timeval tv = {2};
    ASSERT(setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != -1,
           "setsockopt");

    ASSERT((rn = recv(fd, recvbuf, sizeof(recvbuf) - 1, 0)) != -1, "recv");
    if (rn > 0) {
        recvbuf[rn] = '\0';
        printf("data(%d): %s\n", rn, recvbuf);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    argc > 1 ? multicast_receiver() : multicast_sender();
    return 0;
}