#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_FILENAME 128

int main(int argc, char *argv[])
{
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        return 1;
    }
    DIR *dir = opendir(cwd);
    if (dir == NULL)
    {
        perror("Opening directory");
        exit(1);
    }
    struct dirent *data;
    struct stat buffer_stat;
    long long total = 0;
    do
    {
        data = readdir(dir);
        if (data == NULL)
            break;
        stat(data->d_name, &buffer_stat);
        if (!S_ISDIR(buffer_stat.st_mode))
        {
            printf("%ld\t%s\n", buffer_stat.st_size, data->d_name);
            total += buffer_stat.st_size;
        }
    } while (data != NULL);
    printf("%lld\ttotal\n", total);
    closedir(dir);
    return 0;
}