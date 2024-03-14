#define _DEMO_UTILS_TIME
#define _XOPEN_SOURCE
#include "../include/utils.h"

time_t get_timestamp(void)
{
    time_t timer;
    if (time(&timer) == -1)
        return 0;
    return timer;
}

long get_timestamp_millisecond(void)
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == -1)
        return 0;
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

char *get_now_datetime(void)
{
    time_t timer = get_timestamp();
    struct tm now_tm;
    static char _datetime[20];
    localtime_r(&timer, &now_tm);
    strftime(_datetime, sizeof(_datetime), _DEMO_DATETIME_FORMAT, &now_tm);
    return _datetime;
}

time_t parse_datetime(char *datetime_str)
{
    struct tm _tm;
    if (strptime(datetime_str, _DEMO_DATETIME_FORMAT, &_tm) == NULL)
        return 0;
    return mktime(&_tm);
}
