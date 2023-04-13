#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "command.h"

int main(int argc, const char *argv[])
{
    if (check_arguments_validity(argc, argv))
        return 1;

    Command command = get_command(argv[1]);

    if (command != PENDING)
        raise(SIGUSR1);
    else
        check_if_signal_is_pending();

    printf("Program exec ended without exception\n");

    return 0;
}