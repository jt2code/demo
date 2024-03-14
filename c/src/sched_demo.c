#define _GNU_SOURCE
#include "../include/utils.h"
#include <sched.h>
#include <sys/resource.h>

#define USE_SCHED_RR 1
#define _SCHED_POLICY SCHED_RR

int main(int argc, char *argv[])
{
    int prio;
    if (argc > 1)
        prio = atoi(argv[1]);

#if USE_SCHED_RR
    /* check priority range */
    if (prio > sched_get_priority_max(_SCHED_POLICY) ||
        prio < sched_get_priority_min(_SCHED_POLICY)) {
        prio = 1;
    }
    /* set priority for SCHED_RR, ( or SCHED_FIFO) */
    struct sched_param param = {.sched_priority = prio};
    ASSERT(sched_setscheduler(0, _SCHED_POLICY, &param) == 0,
           "sched_setscheduler");
    printf("set priority: %d\n", prio);
#else
    /* check nic value range */
    if (prio < -20 || prio > 19)
        prio = 0;
    /* set nice value for SCHED_OTHER */
    ASSERT(setpriority(PRIO_PROCESS, 0, prio) == 0, "setpriority");
    printf("set nice: %d\n", prio);
#endif

    /* set cpu affinity */
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(3, &cpuset);
    if (sched_setaffinity(0, sizeof(cpuset), &cpuset) != 0) {
        FATAL("sched_setaffinity");
    }

    for (;;)
        ;

    return 0;
}