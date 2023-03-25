#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/** Checking if given arguments are valid arguments (without validating files!) */
int check_arguments_validity(int argc, const char *argv[], int *out_number)
{
    if (argc < 2)
    {
        fprintf(stderr, "Too few arguments passed to program! Should be 1 but were %d\n", argc - 1);
        return 1;
    }

    int number = atoi(argv[1]);
    if (number < 1)
    {
        fprintf(stderr, "Invalid number as argument\n");
        return 2;
    }
    *out_number = number;
    return 0;
}

int main(int argc, const char *argv[])
{
    int number;
    if (check_arguments_validity(argc, argv, &number))
        return 1;

    for (size_t i = 0; i < number; i++)
    {
        int out = fork();
        if (out == 0)
        {
            pid_t current_pid = getpid();
            pid_t parent_pid = getppid();
            printf("PPID: %d\tPID:%d\n", parent_pid, current_pid);
            return 0;
        }
    }

    while (wait(NULL) > 0)
        ;
    printf("%s\n", argv[1]);

    return 0;
}