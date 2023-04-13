#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "queue.c"

struct Queue *queue;

void emit_signal_with_int(int pid, int value)
{
    printf("Sending value %d to process %d\n", value, pid);
    union sigval var;
    var.sival_int = value;
    sigqueue(pid, SIGUSR1, var);
}

void handler(int sig) {}

int check_if_catcher_exists(const char *catcher)
{
    int catcher_pid = atoi(catcher);
    if (catcher_pid == 0)
    {
        fprintf(stderr, "Catcher pid is not a valid number!\n");
        return -1;
    }
    int exists = kill(catcher_pid, 0);
    if (exists == -1)
    {
        fprintf(stderr, "Process with pid %d does not exists!\n", catcher_pid);
        return -1;
    }
    return catcher_pid;
}

void parse_arguments(int argc, const char *argv[])
{
    int command;
    for (int i = 2; i < argc; i++)
    {
        command = atoi(argv[i]);
        if (command < 1 || command > 5)
        {
            printf("Invalid argument at position %d (%s)\n", i, argv[i]);
            continue;
        }
        pushQueue(queue, command);
        if (command == 5)
            return;
    }
}

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Too few arguments passed to program! Should be at least 1 but were %d\n", argc - 1);
        return 1;
    }
    int catcher_pid;
    if ((catcher_pid = check_if_catcher_exists(argv[1])) == -1)
    {
        return 1;
    }
    printf("Hello I am sender, created for: %d\n", catcher_pid);
    queue = initQueue();
    if (signal(SIGUSR1, handler))
    {
        perror("signal");
        exit(1);
    }
    parse_arguments(argc, argv);
    printQueue(queue);
    sigset_t myset;
    sigemptyset(&myset);
    while (1)
    {
        if (queue->size == 0)
            break;
        int val = popQueue(queue);
        emit_signal_with_int(catcher_pid, val);
        sigsuspend(&myset);
    }
    destroyQueue(queue);
    return 0;
}