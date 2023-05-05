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
    srand(time(0));

    int id = atoi(argv[1]);
    printDebugID("BARBER", id, "Fryzjer zaczyna pracę\n");

    SharedMem *sharedMemory = attachSharedMemory();
    if (sharedMemory == 0)
        exit(EXIT_FAILURE);

    sem_t *semBarbers = semaphoreOpen(SEM_BARBERS);
    sem_t *semShmMem = semaphoreOpen(SHARED_MEM);
    sem_t *semChairs = semaphoreOpen(SEM_CHAIRS);
    sem_t *semQueue = semaphoreOpen(SEM_QUEUE);

    while (1)
    {
        // obniżenie semafora zapotrzebowania
        // jeśli semafor > 0 wtedy zmniejsza o 1
        // jeśli semafor == 0 wtedy czeka na nowego klienta
        semaphoreDown(semBarbers);

        // edycja pamięci współdzielonej
        semaphoreUp(semShmMem);
        int clientID, haircut;
        sharedMemPop(sharedMemory, &clientID, &haircut);
        semaphoreDown(semShmMem);

        printDebugID("BARBER", id, "Fryzjer zaczął strzyc klienta ");
        printf("%d z fryzurą %d\n", clientID, haircut);
        fflush(0);

        sleep(HaircutTimes[haircut]);
        printDebugID("BARBER", id, "Fryzjer skończył strzyc klienta ");
        printf("%d z fryzurą %d\n", clientID, haircut);
        fflush(0);

        // po skończonej pracy fryzjer zwalnia krzesło
        semaphoreUp(semChairs);
        // i zwalnia miejsce w kolejce (poczekalni)
        semaphoreUp(semQueue);

        if (sharedMemory->size == 0)
            break;
    }

    detachSharedMemory();

    return EXIT_SUCCESS;
}