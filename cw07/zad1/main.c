#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>

#include "log.h"
#include "sem.h"
#include "defs.h"
#include "shm.h"

int randomTime()
{
    return rand() % 4 + 1;
}

int main(int argc, const char *argv[])
{
    srand(time(0));

    SharedMem *sharedMemory = attachSharedMemory();
    sharedMemory->size = 0;
    sharedMemory->start = 0;

    int semBarber = semaphoreCreate(SEM_BARBERS, 0);
    int semChair = semaphoreCreate(SEM_CHAIRS, 0);
    int semShmMem = semaphoreCreate(SHARED_MEM, 1);
    int semQueue = semaphoreCreate(SEM_QUEUE, CHAIRS);

    printDebug("SIMULATION", "Otwarto salon fryzjerski!\n");
    printDebug("SIMULATION", "Tworzenie fryzjerów!\n");

    char buffer[3];

    for (int i = 0; i < BARBERS; i++)
    {
        if (fork() == 0)
        {
            snprintf(buffer, sizeof(buffer), "%d", i);
            execl("./barber", "barber", buffer, NULL);
        }
    }

    printDebug("SIMULATION", "Tworzenie klientów!\n");

    for (int i = 0; i < CLIENTS; i++)
    {
        if (fork() == 0)
        {
            snprintf(buffer, sizeof(buffer), "%d", i);
            execl("./client", "client", buffer, NULL);
        }
        // sleep(randomTime());
    }

    while (wait(0) > 0)
        ;

    detachSharedMemory();
    destroySharedMemory();

    semaphoreUnlink(SEM_BARBERS);
    semaphoreUnlink(SEM_CHAIRS);
    semaphoreUnlink(SEM_QUEUE);
    semaphoreUnlink(SHARED_MEM);

    printDebug("SIMULATION", "Zamknięcie salonu!\n");

    return 0;
}