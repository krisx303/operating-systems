#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

Container *initialize(int maxSize)
{
    Container *structure = (Container *)malloc(sizeof(Container));
    structure->maxSize = maxSize;
    structure->actualSize = 0;
    structure->strings = (char **)calloc(maxSize, sizeof(char *));
    return structure;
}

int wcToContainer(Container *container, const char *filename)
{
    if (container->actualSize == container->maxSize)
        return STRUCTURE_OVERFLOW;
    // Prepare command like 'wc 'filename.c' >/tmp/tmp_so 2>/tmp/tmp_so'
    char buff[MAX_LINE];
    snprintf(buff, MAX_LINE, "wc '%s' >/tmp/tmp_so 2>/tmp/tmp_so", filename);
    //  Run command
    int file_exist = system(buff);

    if (file_exist != 0)
        return FILE_NOT_FOUND;

    FILE *tmp;

    // Open the file for reading
    tmp = fopen("/tmp/tmp_so", "r");

    // Read the first line
    if (fgets(buff, MAX_LINE, tmp) == NULL)
        return FILE_NOT_FOUND;
    int size = container->actualSize;
    int lineLen = strlen(buff);
    buff[lineLen - 1] = '\0';

    // Prepare memory for wc output line
    container->strings[size] = (char *)calloc(lineLen, sizeof(char));
    // Move line content to new allocated memory block
    memmove(container->strings[size], buff, (lineLen) * sizeof(char));
    container->actualSize++;

    // Close the file
    fclose(tmp);
    remove("/tmp/tmp_so");
    return 0;
}

char *getElementAtIndex(Container *container, int index)
{
    if (index > container->actualSize - 1 || index < 0)
        return (char *)INDEX_OUT_OF_BOUND;
    return container->strings[index];
}

int removeElementAtIndex(Container *container, int index)
{
    if (index > container->actualSize - 1 || index < 0)
        return INDEX_OUT_OF_BOUND;
    free(container->strings[index]);
    for (int i = index; i < container->actualSize - 1; i++)
    {
        container->strings[i] = container->strings[i + 1];
    }

    container->actualSize--;
    return 0;
}

void destroy(Container *container)
{
    for (int i = 0; i < container->actualSize; i++)
    {
        free(container->strings[i]);
    }
    free(container->strings);
    free(container);
}