#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include "lib.h"

#define INIT_PREFIX "init "
#define COUNT_PREFIX "count "
#define SHOW_PREFIX "show "
#define DELETE_PREFIX "delete index "
#define DESTROY_PREFIX "destroy"

int startsWith(const char *str, const char *prefix)
{
    size_t len_prefix = strlen(prefix);
    return strncmp(str, prefix, len_prefix) == 0;
}

int identifyCommand(char *line, size_t len)
{
    if (len < 4)
        return -1;
    if (startsWith(line, INIT_PREFIX))
        return 1;
    if (startsWith(line, COUNT_PREFIX))
        return 2;
    if (startsWith(line, SHOW_PREFIX))
        return 3;
    if (startsWith(line, DELETE_PREFIX))
        return 4;
    if (!strcmp(line, DESTROY_PREFIX))
        return 5;
    return -1;
}

int isDigit(char c)
{
    return c >= '0' && c <= '9';
}

int parseInt(char *line, int startPos, int endPos)
{
    if (startPos == endPos + 1)
        return -1;
    int value = 0;
    int multiplication = 1;
    char c = line[endPos];
    while (endPos >= startPos && isDigit(c))
    {
        value += (c - '0') * multiplication;
        multiplication *= 10;
        endPos--;
        c = line[endPos];
    }
    if (endPos != startPos - 1)
        return -1;
    return value;
}

int getLine(char *buff)
{
    char *out = fgets(buff, MAX_LINE, stdin);
    if (out == NULL)
        return -1;
    size_t len = strlen(buff);
    if (buff[len - 1] == '\n')
    {
        buff[len - 1] = '\0';
        len--;
    }
    return len;
}

int main()
{
    Container *container = NULL;
    char buff[MAX_LINE];
    int len = 0;
    int command;
    double clock_ticks = (double)sysconf(_SC_CLK_TCK);
    printf("> ");
    int index, out;
    struct tms tms_start, tms_end;
    struct timespec real_start, real_end;
    while ((len = getLine(buff)) != -1)
    {
        command = identifyCommand(buff, len);
        if (command > 1 && command < 5 && container == NULL)
        {
            printf("There is no structure in memory, initialize its firstly!\n> ");
            goto time_measure;
        }
        clock_gettime(CLOCK_REALTIME, &real_start);
        times(&tms_start);
        switch (command)
        {
        case 1:
            int size = parseInt(buff, 5, len - 1);
            if (size == -1)
            {
                printf("This size is not valid number!!\n> ");
                goto time_measure;
            }
            if (container == NULL)
            {
                container = initialize(size);
                printf("Structure was created with size: %d\n", size);
            }
            else
                printf("Structure is already initialized\n");
            break;
        case 2:
            out = wcToContainer(container, &buff[6]);
            if (out == STRUCTURE_OVERFLOW)
                printf("The structure is full!\n");
            else if (out == FILE_NOT_FOUND)
                printf("File %s doesn't exist!\n", &buff[6]);
            else
                printf("File %s counted\n", &buff[6]);
            break;
        case 3:
            index = parseInt(buff, 5, len - 1);
            if (index == -1)
            {
                printf("This index is not valid number!!\n> ");
                goto time_measure;
            }

            char *element = getElementAtIndex(container, index);

            if (element == (char *)INDEX_OUT_OF_BOUND)
                printf("Index %d out of bound. There are only %d elements.\n> ", index, container->actualSize);
            else
                printf("%s\n", element);
            break;
        case 4:
            index = parseInt(buff, 13, len - 1);
            if (index == -1)
            {
                printf("This index is not valid number!!\n> ");
                goto time_measure;
            }
            out = removeElementAtIndex(container, index);
            if (out == INDEX_OUT_OF_BOUND)
                printf("Index %d out of bound. There are only %d elements.\n", index, container->actualSize);
            else
                printf("Element at index %d was removed\n", index);
            break;

        case 5:
            if (container == NULL)
            {
                printf("You must initialize structure before destroy!\n");
            }
            else
            {
                destroy(container);
                container = NULL;
                printf("Structure destroyed...\n");
            }

            break;

        default:
            printf("Undefined command!\n");
            break;
        }
    time_measure:
        clock_gettime(CLOCK_REALTIME, &real_end);
        times(&tms_end);
        double time_taken = (real_end.tv_sec - real_start.tv_sec) * 1000.0 + (real_end.tv_nsec - real_start.tv_nsec) / 1000000.0;
        printf("Real   Time: %f ms\n", time_taken);
        printf("System Time: %f s\n", (tms_end.tms_cstime - tms_start.tms_cstime) / clock_ticks);
        printf("User   Time: %f s\n\n", (tms_end.tms_cutime - tms_start.tms_cutime) / clock_ticks);
        printf("> ");
    }
    if (container != NULL)
        destroy(container);
    return 0;
}