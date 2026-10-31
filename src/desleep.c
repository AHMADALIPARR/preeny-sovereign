/* sovereign pure stub: sleep/usleep/nanosleep return immediately */
#include <time.h>

unsigned int sleep(unsigned int seconds)
{
    return 0;
}

unsigned int usleep(unsigned int microseconds)
{
    return 0;
}

int nanosleep(const struct timespec *req, struct timespec *rem)
{
    return 0;
}
