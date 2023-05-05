#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#include "grid.h"

// global variables

const int grid_width = 30;
const int grid_height = 30;

const int grid_size = grid_width * grid_height;

static pthread_t *threads = NULL;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

typedef struct ThreadArgs
{
    char *src;
    char *dst;
    int start; // index of first cell assigned to this thread
    int cells; // number of cells assigned to this thread
} ThreadArgs;

void *thread_main(void *args)
{
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    int position;
    fflush(0);

    while (1)
    {
        position = threadArgs->start;
        for (int cell = 0; cell < threadArgs->cells; cell++)
        {
            threadArgs->dst[position] = is_alive(position / grid_width, position % grid_width, threadArgs->src);
            position++;
        }
        pause();

        char *tmp = threadArgs->src;
        threadArgs->src = threadArgs->dst;
        threadArgs->dst = tmp;
    }

    return NULL;
}

// uruchamianie wątków dla każdej komórki
void run_threads(char *src, char *dst, int noThreads)
{
    threads = malloc(sizeof(pthread_t) * noThreads);

    int cellsPerThread = grid_size / noThreads;

    int restCells = grid_size - (noThreads * cellsPerThread);

    int start = 0;

    for (int i = 0; i < noThreads; i++)
    {
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args->src = src;
        args->dst = dst;
        args->start = start;
        start += cellsPerThread;
        args->cells = cellsPerThread;
        if (i < restCells)
        {
            args->cells++;
            start++;
        }

        pthread_create(
            threads + i,
            NULL,
            thread_main,
            (void *)args);
    }
}

// wysyłanie sygnału do wszystkich wątków aby zaktualizowały swoją wartość
void update_grid(int noThreads)
{
    for (int i = 0; i < noThreads; i++)
    {
        pthread_kill(threads[i], SIGUSR1);
    }
}