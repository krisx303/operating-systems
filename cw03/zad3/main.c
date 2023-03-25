#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>

#ifndef WAIT_FOR_CHILDREN
#define WAIT_FOR_CHILDREN 0
#endif

/** Checking if given arguments are valid arguments (without validating files!) */
int check_arguments_validity(int argc, const char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Too few arguments passed to program! Should be 2 but were %d\n", argc - 1);
        return 1;
    }
    return 0;
}

int is_invalid_dir_name(char *dir_name)
{
    if (strlen(dir_name) > 2)
        return 0;
    if (strcmp(dir_name, "."))
        return 2;
    if (strcmp(dir_name, ".."))
        return 1;
    return 0;
}

int is_string_in_file(const char *file_name, const char *prefix, char *buffer, int len)
{
    int fd = open(file_name, O_RDONLY);
    if (fd == -1)
    {
        perror("Cannot open file");
        return 0;
    }
    int chars = read(fd, buffer, len);
    if (chars == -1)
    {
        perror("Cannot read file");
        return 0;
    }
    close(fd);
    if (chars < len)
        return 0;

    if (!strncmp(buffer, prefix, len))
    {
        return 1;
    }
    return 0;
}

void search_directory(const char *dir_path, const char *prefix)
{
    DIR *dir = opendir(dir_path);
    struct dirent *data;
    struct stat buffer_stat;
    char new_dir_path[PATH_MAX];
    int len = strlen(prefix);
    char content[len];
    int pid, out;
    if (dir == NULL)
        return;
    while ((data = readdir(dir)) != NULL)
    {
        if (is_invalid_dir_name(data->d_name))
            continue;
        sprintf(new_dir_path, "%s/%s", dir_path, data->d_name);
        out = stat(new_dir_path, &buffer_stat);
        if (out == -1)
        {
            perror("Stat cannot get information about file");
            continue;
        }
        if (!S_ISDIR(buffer_stat.st_mode))
        {
            if (is_string_in_file(new_dir_path, prefix, content, len))
                printf("%s\n", new_dir_path);
        }
        else
        {
            if ((pid = fork()) == 0)
            {
                search_directory(new_dir_path, prefix);
                return;
            }
            else if (pid == 0)
            {
                perror("Cannot fork process");
                return;
            }
        }
    }
    fflush(stdout);
    closedir(dir);
}

int main(int argc, const char *argv[])
{
    if (check_arguments_validity(argc, argv))
        return 1;

    search_directory(argv[1], argv[2]);
    if (WAIT_FOR_CHILDREN)
    {
        while (wait(NULL) > 0)
            ;
    }
    return 0;
}