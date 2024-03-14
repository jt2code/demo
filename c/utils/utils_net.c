
#include <string.h>

#define _DEMO_UTILS_NET
#include "../include/utils.h"

int init_sockaddr_in(SA_IN *addr, char *host, uint16_t port)
{
    bzero(addr, sizeof(SA_IN));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    return inet_pton(AF_INET, host, &(addr->sin_addr.s_addr));
}

int sprintf_sockaddr(SA_IN *addr, char *buf, size_t size)
{
    char host[16];
    u_int16_t port;
    inet_ntop(AF_INET, &(addr->sin_addr.s_addr), host, sizeof(SA_IN));
    port = ntohs(addr->sin_port);
    return snprintf(buf, size, "%s:%d", host, port);
}
