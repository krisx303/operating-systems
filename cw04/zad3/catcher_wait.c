#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int action;
int modes;
int sender_pid;
int mode_changed;

void signal_int_handler(int signum, siginfo_t *info, void *unknown)
{
    action = info->si_value.sival_int;
    modes++;
    sender_pid = info->si_pid;
    mode_changed = 1;
}

void set_signal_action()
{
    struct sigaction act;
    act.sa_sigaction = signal_int_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
}

void print_time()
{
    time_t raw_time;
    struct tm *time_info;
    char time_string[9];

    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(time_string, sizeof(time_string), "%H:%M:%S", time_info);

    printf("%s!\n", time_string);
}

int main(int argc, const char *argv[])
{
    int pid = getpid();
    printf("Hello, I am catcher with pid %d\n", pid);
    int make_loop = 0;
    set_signal_action();
    sigset_t myset;
    sigemptyset(&myset);
    while (1)
    {
        if (!make_loop)
            sigsuspend(&myset);
        make_loop = 0;
        printf("do job for action %d\n", action);
        mode_changed = 0;
        switch (action)
        {
        case 1:
            for (int i = 1; i < 101; i++)
                printf("%d ", i);
            printf("\n");
            kill(sender_pid, SIGUSR1);
            break;

        case 2:
            print_time();
            kill(sender_pid, SIGUSR1);
            break;

        case 3:
            printf("mode was changed %d times\n", modes);
            kill(sender_pid, SIGUSR1);
            break;
        case 4:
            kill(sender_pid, SIGUSR1);
            while (action == 4 && !mode_changed)
            {
                print_time();
                sleep(1);
                make_loop = 1;
            }

            break;
        case 5:
            kill(sender_pid, SIGUSR1);
            return 0;
            break;
        }
    }

    return 0;
}