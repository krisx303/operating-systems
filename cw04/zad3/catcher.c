#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int action;
int modes;
int mode_changed;

void signal_int_handler(int signum, siginfo_t *info, void *unknown)
{
    if (info->si_value.sival_int != action)
    {
        action = info->si_value.sival_int;
        mode_changed = 1;
    }
    modes++;
    kill(info->si_pid, SIGUSR1);
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

    set_signal_action();
    sigset_t myset;
    int make_loop = 0;
    sigemptyset(&myset);
    while (1)
    {
        if (!make_loop)
            sigsuspend(&myset);
        make_loop = 0;
        int act = action, changes = modes;
        // printf("do job for action %d\n", act);
        mode_changed = 0;
        switch (act)
        {
        case 1:
            for (int i = 1; i < 101; i++)
                printf("%d ", i);
            printf("\n");
            break;

        case 2:
            print_time();
            break;

        case 3:
            printf("mode was changed %d times\n", changes);
            fflush(stdout);
            break;

        case 4:
            while (action == 4 && !mode_changed)
            {
                print_time();
                sleep(1);
                make_loop = 1;
            }
            break;

        case 5:
            return 0;
            break;

        default:
            printf("asd\n");
            break;
        }
    }

    return 0;
}