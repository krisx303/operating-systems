#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef MEASURES
#define MEASURES 1
#endif
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif
#define MAX_FILENAME 128

/** Checking if given arguments are valid arguments (without validating files!) */
int check_arguments_validity(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Too few arguments passed to program! Should be 2 but were %d\n", argc - 1);
        return 1;
    }
    return 0;
}

/** Opens file by given @filename with @mode*/
FILE *open_file(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if (file == NULL)
    {
        fprintf(stderr, "File with name '%s' cannot be opened with mode '%s'\n", filename, mode);
        perror("Error");
        return NULL;
    }
    return file;
}

void reverse_chars_one_by_one(FILE *src, FILE *dst)
{
    char c;
    fseek(src, 0, SEEK_END);
    long int len = ftell(src);
    for (size_t i = 0; i < len; i++)
    {
        fseek(src, -(i + 1), SEEK_END);
        if (fread(&c, sizeof(char), 1, src) != 1)
        {
            perror("Reading file byte by byte");
            break;
        }
        fwrite(&c, sizeof(char), 1, dst);
    }
}

void _reverse_chars(FILE *src, FILE *dst, int size, char *buffer)
{
    if (fread(buffer, sizeof(char), size, src) != size)
    {
        perror("Reading file buffer");
    }
    int left = 0;
    int right = size - 1;
    while (left < right)
    {
        char tmp = buffer[left];
        buffer[left] = buffer[right];
        buffer[right] = tmp;
        left++;
        right--;
    }
    fwrite(buffer, sizeof(char), size, dst);
}

void reverse_chars_buffer(FILE *src, FILE *dst)
{
    char buffer[BUFFER_SIZE];
    fseek(src, 0, SEEK_END);
    long int len = ftell(src);
    for (size_t i = 0; i < len / BUFFER_SIZE; i++)
    {
        fseek(src, -(i + 1) * BUFFER_SIZE, SEEK_END);
        _reverse_chars(src, dst, BUFFER_SIZE, buffer);
    }
    fseek(src, 0, SEEK_SET);
    _reverse_chars(src, dst, len % BUFFER_SIZE, buffer);
}

int main(int argc, char *argv[])
{
    if (check_arguments_validity(argc, argv))
        return 1;
    double time_taken;
    struct timespec real_start, real_end;
    FILE *src = open_file(argv[1], "r");
    if (src == NULL)
    {
        return 1;
    }
    FILE *dst = open_file(argv[2], "w");
    if (dst == NULL)
    {
        fclose(dst);
        return 2;
    }

    for (size_t i = 0; i < MEASURES; i++)
    {

        fseek(dst, 0, SEEK_SET);

        clock_gettime(CLOCK_REALTIME, &real_start);

        if (BUFFER_SIZE == 1)
            reverse_chars_one_by_one(src, dst);
        else
            reverse_chars_buffer(src, dst);

        clock_gettime(CLOCK_REALTIME, &real_end);
        time_taken += (real_end.tv_sec - real_start.tv_sec) * 1000.0 + (real_end.tv_nsec - real_start.tv_nsec) / 1000000.0;
    }
    fclose(src);
    fclose(dst);
    printf("Average time: %f ms\n", time_taken / MEASURES);
    return 0;
}