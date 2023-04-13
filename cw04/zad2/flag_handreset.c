#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void sigint_handler(int sig)
{
    printf("Received SIGUSR1 signal\n");
}

int main()
{
    printf("Setting signal handler\n");
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESETHAND;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("Sending first signal (expecting caught by handler)\n");

    raise(SIGUSR1);

    printf("Sending second signal (expecting error)\n");

    raise(SIGUSR1);
    return 0;
}