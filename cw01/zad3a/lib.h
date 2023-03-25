#ifndef lib_h__
#define lib_h__

#define MAX_LINE 128
#define STRUCTURE_OVERFLOW 2
#define FILE_NOT_FOUND 3
#define INDEX_OUT_OF_BOUND -1

typedef struct Container
{
    char **strings;
    int maxSize;
    int actualSize;
} Container;

/**
 * Initializes Container structure with default values and given array size
 *
 * @param maxSize is the size of Container
 * @return The created Container structure
 */
extern Container *initialize(int maxSize);

/**
 * Runs wc command on given filename and add output to container
 *
 * @param container is pointer to existing Container instance
 * @param filename is filename on which will be executed wc command
 * @return 0 when everything is fine
 * @note when file with name/path 'filename' doesn't exist then returns FILE_NOT_FOUND
 * @note when structure is full then returns STRUCTURE_OVERFLOW
 */
extern int wcToContainer(Container *container, const char *filename);

/**
 * Gets element from container at given index
 *
 * @param container is pointer to existing Container instance
 * @param index is pointing which element should be returned
 *
 * @return element at given index or INDEX_OF_OUT_BOUND when index is not accessible
 */
extern char *getElementAtIndex(Container *container, int index);

/**
 * Removes element from container at given index
 *
 * @param container is pointer to existing Container instance
 * @param index is pointing which element should be deleted
 *
 * @return 0 when everything is fine
 * @note when index is not accessible then returns INDEX_OF_OUT_BOUND
 */
extern int removeElementAtIndex(Container *container, int index);

/**
 * Destroys the given container and deleting all containing pointers
 *
 * @param container is pointer to structure to delete
 */
extern void destroy(Container *container);

#endif // lib_h__