#include "command.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

Command get_command(const char arg[])
{
    if (!strcmp(arg, "ignore"))
        return IGNORE;
    if (!strcmp(arg, "handler"))
        return HANDLER;
    if (!strcmp(arg, "mask"))
        return MASK;
    if (!strcmp(arg, "pending"))
        return PENDING;
    return INVALID;
}

int check_arguments_validity(int argc, const char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Too few arguments passed to program! Should be 1 but were %d\n", argc - 1);
        return 1;
    }
    return 0;
}

void check_if_signal_is_pending()
{
    sigset_t pending_set;
    sigpending(&pending_set);
    if (sigismember(&pending_set, SIGUSR1))
    {
        printf("signal USR1 is pending for %d process\n", getpid());
    }
    else
    {
        printf("there is no USR1 signal on pending list for process %d\n", getpid());
    }
}