#include <signal.h>
#include <stdio.h>
#include <unistd.h>

typedef void (*ActionHandler)(int signum, siginfo_t *info, void *p3);

void emit_signal_with_int(int pid, int value)
{
    printf("Sending value %d to process %d\n", value, pid);
    union sigval var;
    var.sival_int = value;
    sigqueue(pid, SIGUSR1, var);
}

void emit_signal_with_string(int pid, char *string)
{
    printf("Sending value %s to process %d\n", string, pid);
    union sigval var;
    var.sival_ptr = string;
    sigqueue(pid, SIGUSR1, var);
}

void signal_int_handler(int signum, siginfo_t *info, void *p3)
{
    printf("Caught signal from pid %d with value %d\n", info->si_pid, info->si_value.sival_int);
}

void signal_data_handler(int signum, siginfo_t *info, void *p3)
{
    printf("Caught from pid %d with string value %s\n", info->si_pid, (char *)info->si_value.sival_ptr);
}

void set_signal_action(ActionHandler handler)
{
    struct sigaction act;
    act.sa_sigaction = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
}

int main(int argc, const char **argv)
{
    int pid = getpid();
    printf("Current pid: %d\n", pid);

    printf("\nScenario 1\nProgram will send signal with number to itself and will catch it in the handler\n");
    set_signal_action(signal_int_handler);
    emit_signal_with_int(pid, 321);

    printf("\nScenario 2\nProgram will send signal with data to itself and will catch it in the handler\n");
    set_signal_action(signal_data_handler);
    emit_signal_with_string(pid, "hello");

    printf("\nScenario 3\nProgram will send signal from forked child and receive that in main program handler\n");
    set_signal_action(signal_int_handler);
    int asd = fork();
    if (asd != 0)
        emit_signal_with_int(asd, 12350);

    return 0;
}