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
#ifdef SYS
#define PREFIX "sys"
#else
#define PREFIX "std"
#endif
#define MAX_FILENAME 1024

typedef void FileDescriptor;

typedef enum
{
    READ_FILE,
    WRITE_FILE
} FileMode;

/** Checking if argument on position @position is single character  */
int check_argument_is_char(char *arg, int position)
{
    if (strlen(arg) > 1)
    {
        fprintf(stderr, "Argument at position %d: '%s' should be single character!\n", position, arg);
        return 1;
    }
    return 0;
}

/** Checking if given arguments are valid arguments (without validating files!) */
int check_arguments_validity(int argc, char *argv[])
{
    if (argc < 5)
    {
        fprintf(stderr, "Too few arguments passed to program! Should be 4 but were %d\n", argc - 1);
        return 1;
    }
    if (check_argument_is_char(argv[1], 1))
        return 1;
    if (check_argument_is_char(argv[2], 2))
        return 1;
    return 0;
}

/** Opens file by given filename and mode.
 * Implementation is depended on declaration of SYS constant!!
 */
FileDescriptor *open_file(const char *filename, FileMode mode)
{
#ifdef SYS
    int fd;
    if (mode == READ_FILE)
        fd = open(filename, O_RDONLY);
    else
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1)
    {
        return NULL;
    }
    int *pfd = (int *)malloc(sizeof(int));
    *pfd = fd;
    return pfd;
#else
    if (mode == READ_FILE)
        return (FileDescriptor *)fopen(filename, "r");
    return (FileDescriptor *)fopen(filename, "w");
#endif
}

/** Closes file by given FileDescriptor.
 * Implementation is depended on declaration of SYS constant!!
 */
void close_file(FileDescriptor *file)
{
#ifdef SYS
    close(*((int *)file));
    free(file);
#else
    fclose((FILE *)file);
#endif
}

/** Reads @buffer_size characters from @file to @buffer
 * Implementation is depended on declaration of SYS constant!!
 */
int file_read(FileDescriptor *file, void *buffer, size_t buffer_size)
{
#ifdef SYS
    return read(*((int *)file), buffer, buffer_size);
#else
    return fread(buffer, sizeof(char), buffer_size, (FILE *)file);
#endif
}

/** Writes @count characters from @buffer to @file.
 * Implementation is depended on declaration of SYS constant!!
 */
void file_write(FileDescriptor *file, void *buffer, size_t count)
{
#ifdef SYS
    write(*((int *)file), buffer, count);
#else
    fwrite(buffer, sizeof(char), count, file);
#endif
}

/** Opens file by given filename and mode.
 * Implementation is depended on declaration of SYS constant!!
 */
void file_seek_start(FileDescriptor *file)
{
#ifdef SYS
    lseek(*((int *)file), 0, SEEK_SET);
#else
    fseek((FILE *)file, 0, SEEK_SET);
#endif
}

/** Function which replaces characters in file.  */
void replace_chars(char inChar, char outChar, FileDescriptor *src, FileDescriptor *dst)
{
    char buffer[BUFFER_SIZE];
    int count = -1;
    do
    {
        count = file_read(src, &buffer, BUFFER_SIZE);
        for (size_t i = 0; i < count; i++)
        {
            if (buffer[i] == inChar)
                buffer[i] = outChar;
        }
        file_write(dst, &buffer, count);

    } while (count);
}

int main(int argc, char *argv[])
{
    if (check_arguments_validity(argc, argv))
        return 1;

    FileDescriptor *src = open_file(argv[3], READ_FILE);
    if (src == NULL)
    {
        fprintf(stderr, "File with name '%s' cannot be opened with read mode\n", argv[3]);
        perror("Error");
        return 2;
    }
    FileDescriptor *dst = open_file(argv[4], WRITE_FILE);
    if (dst == NULL)
    {
        fprintf(stderr, "File with name '%s' cannot be opened with write mode\n", argv[4]);
        perror("Error");
        close_file(src);
        return 3;
    }

    double time_taken;
    struct timespec real_start, real_end;

    for (size_t i = 0; i < MEASURES; i++)
    {
        clock_gettime(CLOCK_REALTIME, &real_start);
        replace_chars(argv[1][0], argv[2][0], src, dst);

        clock_gettime(CLOCK_REALTIME, &real_end);
        time_taken += (real_end.tv_sec - real_start.tv_sec) * 1000.0 + (real_end.tv_nsec - real_start.tv_nsec) / 1000000.0;
        file_seek_start(src);
        file_seek_start(dst);
        // printf("Time: %f ms\n", time_taken);
    }
    printf("Average %s time: %f ms\n", PREFIX, time_taken / MEASURES);

    close_file(src);
    close_file(dst);

    return 0;
}