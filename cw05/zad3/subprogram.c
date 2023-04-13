#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

double f(double x)
{
    return 4 / (x * x + 1);
}

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

int main(int argc, const char *argv[])
{
    double start = atof(argv[1]);
    double end = atof(argv[2]);
    double interval = atof(argv[3]);

    double value = compute_integral(start, end, interval);

    int named_pipe = open("/tmp/integral_fifo", O_WRONLY);
    write(named_pipe, &value, sizeof(double));
    close(named_pipe);
    return 0;
}