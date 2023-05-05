#pragma once

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdlib.h>

typedef struct SharedElement
{
    int haircut;
    int clientID;
} SharedElement;

typedef struct SharedMem
{
    int start;
    int size;
    SharedElement data[QUEUE_SIZE];
} SharedMem;

void sharedMemPop(SharedMem *sharedMemory, int *clientID, int *haircut)
{
    int index = sharedMemory->start;
    *clientID = sharedMemory->data[index].clientID;
    *haircut = sharedMemory->data[index].haircut;
    sharedMemory->size--;
    sharedMemory->start = (sharedMemory->start + 1) % (QUEUE_SIZE);
}

void sharedMemPush(SharedMem *sharedMemory, int clientID, int haircut)
{
    int index = (sharedMemory->start + sharedMemory->size) % (QUEUE_SIZE);
    sharedMemory->data[index].clientID = clientID;
    sharedMemory->data[index].haircut = haircut;
    sharedMemory->size++;
}

static int getSharedMemory(int size)
{
    key_t key = ftok(getenv("HOME"), 0);
    if (key == -1)
        return -1;
    return shmget(key, size, 0666 | IPC_CREAT);
}

SharedMem *attachSharedMemory()
{
    int id = getSharedMemory(sizeof(SharedMem));
    if (id == -1)
        return (SharedMem *)-1;
    return (SharedMem *)shmat(id, NULL, 0);
}

int detachSharedMemory()
{
    return shmdt(getenv("HOME"));
}

int destroySharedMemory()
{
    int id = getSharedMemory(0);
    if (id == -1)
        return -1;
    return shmctl(id, IPC_RMID, NULL);
}