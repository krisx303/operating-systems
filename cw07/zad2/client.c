#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>

#include "sem.h"
#include "log.h"
#include "defs.h"
#include "shm.h"

int main(int argc, const char *argv[])
{
    srand(time(0) + getpid());

    int id = atoi(argv[1]);
    printDebugID("CLIENT", id, "Klient przychodzi do salonu\n");

    SharedMem *sharedMemory = attachSharedMemory();
    if (sharedMemory == 0)
        exit(EXIT_FAILURE);

    sem_t *semBarbers = semaphoreOpen(SEM_BARBERS);
    sem_t *semShmMem = semaphoreOpen(SHARED_MEM);
    sem_t *semChairs = semaphoreOpen(SEM_CHAIRS);
    sem_t *semQueue = semaphoreOpen(SEM_QUEUE);

    semaphoreDown(semShmMem);
    int size = sharedMemory->size;

    if (size >= (QUEUE_SIZE))
    {
        printDebugID("CLIENT", id, "Poczekalnia jest pełna, wychodzę!\n");
        semaphoreUp(semShmMem);
        exit(EXIT_SUCCESS);
    }

    sharedMemPush(sharedMemory, id, rand() % HAIRCUTS);
    semaphoreUp(semShmMem);
    // jeśli jest miejsce w kolejce to klient 'wrzuca się do kolejki', tzn obniża semafor queue czekając na fryzjera
    semaphoreDown(semQueue);

    // zwiększa zapotrzebowanie na fryzjera
    semaphoreUp(semBarbers);
    // gdy pojawi się fryzjer zajmuje krzesło, lub czeka na zwolnienie krzesła
    semaphoreDown(semChairs);

    printDebugID("CLIENT", id, "Klient wychodzi!\n");

    detachSharedMemory();

    return EXIT_SUCCESS;
}