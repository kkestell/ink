#include <signal.h>

void (*signal(int sig, void (*func)(int)))(int)
{
}

int raise(int sig)
{
    return 0;
}
