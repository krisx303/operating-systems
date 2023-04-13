#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

#define MAX_SUB_PROCESS 256
#define FIFO_PATH "/tmp/integral_fifo"

int validate_arguments(int argc, const char *args[], double *ivt, int *sbprc)
{
    if (argc < 3)
    {
        fprintf(stderr, "Too few arguments!\n");
        return -1;
    }

    double interval = atof(args[1]);
    if (interval == 0.0)
    {
        fprintf(stderr, "Entered interval is 0 or is not a number!\n");
        return -2;
    }

    int sub_process = atoi(args[2]);
    if (sub_process < 1)
    {
        fprintf(stderr, "Number of subprocesses must be positive!\n");
        return -3;
    }
    else if (sub_process > MAX_SUB_PROCESS)
    {
        fprintf(stderr, "Number of subprocesses can't be bigger than %d!\n", MAX_SUB_PROCESS);
        return -4;
    }

    *ivt = interval;
    *sbprc = sub_process;
    return 0;
}

void write_to_file(FILE *file, int n, double interval, double sum, double time_taken)
{
    fprintf(file, "For %d subprocesses and %lf interval\n", n, interval);
    fprintf(file, "Computed integral: %lf\n", sum);
    fprintf(file, "In %lf miliseconds\n", time_taken);
    fflush(file);
}

int main(int argc, const char *argv[])
{
    struct timespec real_start, real_end;

    double interval;
    int sub_processes;
    if (validate_arguments(argc, argv, &interval, &sub_processes))
        return -1;

    if (mkfifo(FIFO_PATH, 0666) == -1)
    {
        perror("Error creating FIFO");
        return -1;
    }

    double width = 1.0 / sub_processes;

    char arg2[128], arg3[128];

    clock_gettime(CLOCK_REALTIME, &real_start);

    for (int i = 0; i < sub_processes; i++)
    {
        if (fork() == 0)
        {
            sprintf(arg2, "%lf", i * width);
            sprintf(arg3, "%lf", (i + 1) * width);
            execl("./subprogram", "subprogram", arg2, arg3, argv[1], NULL);
            perror("Error running subprogram");
            exit(EXIT_FAILURE);
        }
    }
    int fifo = open(FIFO_PATH, O_RDONLY);

    // wait for all children
    while (wait(0) > 0)
        ;

    double sum = 0.0, result;

    for (int i = 0; i < sub_processes; i++)
    {
        read(fifo, &result, sizeof(double));
        sum += result;
    }

    clock_gettime(CLOCK_REALTIME, &real_end);

    close(fifo);

    remove(FIFO_PATH);

    double time_taken = (real_end.tv_sec - real_start.tv_sec) * 1000.0 + (real_end.tv_nsec - real_start.tv_nsec) / 1000000.0;

    write_to_file(stdout, sub_processes, interval, sum, time_taken);
    FILE *file = fopen("report.txt", "a+");
    write_to_file(file, sub_processes, interval, sum, time_taken);
    fclose(file);
    return 0;
}