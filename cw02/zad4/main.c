#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ftw.h>

#define MAX_FILENAME 128

long long int total_size_files = 0;

void check_input_validity(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Too few arguments passed to program! Should be 1 but were %d\n", argc - 1);
        exit(1);
    }
}

int print_file_name(const char *filename, const struct stat *ptr, int flag)
{
    if (S_ISDIR(ptr->st_mode))
        return 0;
    int len = strlen(filename);
    size_t i;
    for (i = len - 1; i > 0; i--)
    {
        if (filename[i] == '/')
            break;
    }

    printf("%ld\t%s\n", ptr->st_size, &(filename[i + 1]));
    total_size_files += ptr->st_size;
    return 0;
}

int main(int argc, char *argv[])
{
    check_input_validity(argc, argv);
    ftw(argv[1], print_file_name, 100);
    printf("Total size of files in '%s' directory: %lld\n", argv[1], total_size_files);
    return 0;
}

// int print_file(const char *filename, const struct stat *ptr, int flag)
// {
//     if (!S_ISDIR(ptr->st_mode))
//     {
//         printf("%ld\t%s\n", ptr->st_size, filename);
//         total_size_files += ptr->st_size;
//     }
//     return 0;
// }