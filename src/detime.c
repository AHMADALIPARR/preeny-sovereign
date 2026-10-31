/* sovereign pure stub: time and gettimeofday return fixed zero values */
#include <time.h>

time_t time(time_t *tloc)
{
    if (tloc != 0) *tloc = 0;
    return 0;
}

int gettimeofday(struct timeval *tv, void *tz)
{
    if (tv) {
        tv->tv_sec = 0;
        tv->tv_usec = 0;
    }
    return 0;
}
