#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sighandler(int signo, siginfo_t *siginfo, void *sth)
{
    char *data = siginfo->si_value.sival_ptr;
    printf("Received data '%s' with signal %d\n", data, signo);
    fflush(stdout);
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;
    action.sa_sigaction = &sighandler;

    //..........

    sigemptyset(&action.sa_mask);

    action.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR1, &action, NULL) == -1)
    {
        perror("sigaction SIGUSR1");
        return 1;
    }
    if (sigaction(SIGUSR2, &action, NULL) == -1)
    {
        perror("sigaction SIGUSR2");
        return 1;
    }

    int child = fork();
    if (child == 0)
    {
        // zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1 i SIGUSR2
        // zdefiniuj obsluge SIGUSR1 i SIGUSR2 w taki sposob zeby proces potomny wydrukowal
        // na konsole przekazana przez rodzica wraz z sygnalami SIGUSR1 i SIGUSR2 wartosci
        sigset_t set;
        sigfillset(&set);
        sigdelset(&set, SIGUSR1);
        sigdelset(&set, SIGUSR2);
        if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
        {
            perror("sigprocmask");
            return 1;
        }
    }
    else
    {
        // wyslij do procesu potomnego sygnal przekazany jako argv[2]
        // wraz z wartoscia przekazana jako argv[1]
        int signo = atoi(argv[2]);
        if (signo == 0)
        {
            fprintf(stderr, "'%s' is wrong signal number", argv[2]);
            return 2;
        }
        union sigval value;
        value.sival_ptr = argv[1];
        sigqueue(child, signo, value);
    }

    return 0;
}
