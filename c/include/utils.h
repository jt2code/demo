#ifndef _DEMO_UTILS_H
#define _DEMO_UTILS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef _DEMO_UTILS_NET

#include <arpa/inet.h>

typedef struct sockaddr SA;
typedef struct sockaddr_in SA_IN;

int init_sockaddr_in(SA_IN *addr, char *host, uint16_t port);
int sprintf_sockaddr(SA_IN *addr, char *buf, size_t size);

#endif

#ifdef _DEMO_UTILS_TIME

#include <sys/time.h>
#include <time.h>

#define _DEMO_DATETIME_FORMAT "%F %T" /* %Y-%m-%d %H:%M:%S */
time_t get_timestamp(void);
long get_timestamp_millisecond(void);
char *get_now_datetime(void);
time_t parse_datetime(char *datetime_str);

#endif

#define FATAL(errmsg)                                                          \
    do {                                                                       \
        perror(errmsg);                                                        \
        _exit(EXIT_FAILURE);                                                   \
    } while (0)

#define ASSERT(cond, errmsg)                                                   \
    do {                                                                       \
        if (!(cond)) {                                                         \
            FATAL(errmsg);                                                     \
        }                                                                      \
    } while (0)

#define FATAL_ERRNO(_errno, errmsg)                                            \
    errno = _errno;                                                            \
    FATAL(errmsg);

#endif