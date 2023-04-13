#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_SUB_PROCESS 256

/** Validating input arguments and converting values */
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

/** Mathematic function for integral  */
double f(double x)
{
    return 4 / (x * x + 1);
}

/** Approximated riemann integral by rectangles */
double compute_integral(double start, double end, double interval)
{
    double result = 0.0;
    while (start - end < 0)
    {
        result += interval * f(start);
        start += interval;
    }
    return result;
}

/** Reading computed fragments from pipes and sum it up */
double sum_outputs(int pipes[], int n)
{
    double result, sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        read(pipes[i], &result, sizeof(double));
        sum += result;
    }
    return sum;
}

void write_to_file(FILE *file, int n, double interval, double sum, double time_taken)
{
    fprintf(file, "For %d subprocesses and %lf interval\n", n, interval);
    fprintf(file, "Computed integral: %lf\n", sum);
    fprintf(file, "In %lf miliseconds\n", time_taken);
    fflush(file);
}

int main(int argc, const char *args[])
{
    struct timespec real_start, real_end;

    double interval;
    int sub_processes;
    if (validate_arguments(argc, args, &interval, &sub_processes))
        return -1;

    double width = 1.0 / sub_processes;

    int pipes[sub_processes];
    double start, end, result;

    clock_gettime(CLOCK_REALTIME, &real_start);

    for (int i = 0; i < sub_processes; i++)
    {
        int fd[2];
        start = i * width;
        end = (i + 1) * width;
        pipe(fd);
        if (fork() == 0)
        {
            // child process
            close(fd[0]);
            result = compute_integral(start, end, interval);
            write(fd[1], &result, sizeof(double));
            return 0;
        }
        else
        {
            // parent process
            close(fd[1]);
            pipes[i] = fd[0];
        }
    }

    // wait for all children
    while (wait(0) > 0)
        ;

    double sum = sum_outputs(pipes, sub_processes);

    clock_gettime(CLOCK_REALTIME, &real_end);

    double time_taken = (real_end.tv_sec - real_start.tv_sec) * 1000.0 + (real_end.tv_nsec - real_start.tv_nsec) / 1000000.0;

    write_to_file(stdout, sub_processes, interval, sum, time_taken);
    FILE *file = fopen("report.txt", "a+");
    write_to_file(file, sub_processes, interval, sum, time_taken);
    fclose(file);
    return 0;
}