#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#ifndef USE_FLAG
#define USE_FLAG 0
#endif

void sigint_handler(int sig)
{
    printf("(parent) Received SIGINT signal (%d)\n", sig);
}

int main()
{
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    if (!USE_FLAG)
        sa.sa_flags = 0;
    else
        sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    int pid = fork();

    if (pid == 0)
    {
        printf("(child) Child goes to sleep\n");
        sleep(1);
        printf("(child) Sending signal to parent:\n");
        kill(getppid(), SIGINT);
        printf("(child) Child goes to sleep\n");
        sleep(1);
        printf("(child) Program ended\n");
    }
    else
    {
        printf("(parent) Parent is waiting\n");
        while (wait(0) > 0)
            ;
        printf("(parent) Parent passed wait\n");
        int exists = kill(pid, 0);
        if (exists == 0)
        {
            printf("(parent) child is still running\n");
            kill(pid, SIGKILL);
        }
        else
        {
            printf("(parent) child is ended\n");
        }
    }
    return 0;
}