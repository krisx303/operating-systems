#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/** Checking if given arguments are valid arguments (without validating files!) */
int check_arguments_validity(int argc, const char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Too few arguments passed to program! Should be 1 but were %d\n", argc - 1);
        return 1;
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    if (check_arguments_validity(argc, argv))
        return 1;

    printf("%s ", argv[0]);
    fflush(stdout);
    execl("/bin/ls", "/bin/ls", argv[1], NULL);
    return 0;
}