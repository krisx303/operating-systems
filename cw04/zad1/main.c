#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "command.h"

// Functions for setting behaviour when signal was received

void handler(int signo)
{
    printf("Caught signal %d in process %d\n", signo, getpid());
}

void set_mask()
{
    sigset_t new_mask, old_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);
}

int set_behaviour_from_command(Command command)
{
    switch (command)
    {
    case IGNORE:
        signal(SIGUSR1, SIG_IGN);
        break;

    case HANDLER:
        signal(SIGUSR1, handler);
        break;

    case PENDING:
    case MASK:
        set_mask();
        break;

    default:
        printf("Invalid command argument\n");
        return 1;
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    if (check_arguments_validity(argc, argv))
        return 1;

    Command command = get_command(argv[1]);

    if (set_behaviour_from_command(command) == 1)
        return 1;

    printf("Main pid: %d\n", getpid());

    raise(SIGUSR1);
    if (command == PENDING)
        check_if_signal_is_pending();

    int pid = fork();
    if (pid != 0)
        goto run_exec;
    printf("Fork pid: %d\n", getpid());

    if (command != PENDING)
        raise(SIGUSR1);
    else
        check_if_signal_is_pending();

    printf("Forked program ended without exception\n");
    return 0;

run_exec:
    wait(NULL);
    if (command != HANDLER)
    {
        printf("Exec pid: %d\n", getpid());
        fflush(stdout);
        execl("./main_exec", "main_exec", argv[1], NULL);
    }

    return 0;
}