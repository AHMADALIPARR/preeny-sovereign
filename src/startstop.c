/* sovereign pure stub: stop the process on load */
#define _GNU_SOURCE
#include <signal.h>
#include <unistd.h>

__attribute__((constructor)) void preeny_startstop(void)
{
    kill(getpid(), SIGSTOP);
}
