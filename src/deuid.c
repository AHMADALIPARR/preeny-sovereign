/* sovereign pure stub: getuid/geteuid always return 0 */
#include <unistd.h>

uid_t getuid(void)
{
    return 0;
}

uid_t geteuid(void)
{
    return 0;
}
