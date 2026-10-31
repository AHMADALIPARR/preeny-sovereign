/* sovereign pure stub: fork always returns 0 (child never spawned) */
#include <unistd.h>

pid_t fork(void)
{
    return 0;
}
