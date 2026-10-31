/* sovereign pure stub: sigaction always succeeds and does nothing */
#include <signal.h>

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)
{
    return 0;
}
