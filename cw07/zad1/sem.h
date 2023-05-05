#pragma once

#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>

struct sembuf operationDown = {0, -1, 0};
struct sembuf operationUp = {0, 1, 0};

key_t generateKey(char project)
{
    key_t key = ftok(getenv("HOME"), project);
    if (key == -1)
        perror("generate key");
    return key;
}

int semaphoreCreate(char project, int initial)
{
    key_t key = generateKey(project);
    if (key == -1)
        return -1;
    int semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1)
    {
        perror("create semaphore");
        return -1;
    }
    if (semctl(semid, 0, SETVAL, initial) == -1)
    {
        perror("set initial value to semaphore");
        return -1;
    }
    return semid;
}

int semaphoreOpen(char project)
{
    key_t key = generateKey(project);
    if (key == -1)
        return -1;
    return semget(key, 1, 0);
}

void semaphoreUnlink(char project)
{
    int semid = semaphoreOpen(project);
    if (semid == -1)
        return;
    if (semctl(semid, 0, IPC_RMID) == -1)
        perror("unlink semaphore");
}

void semaphoreDown(int semid)
{
    if (semop(semid, &operationDown, 1) == -1)
        perror("semaphore down");
}

void semaphoreUp(int semid)
{
    if (semop(semid, &operationUp, 1) == -1)
        perror("semaphore up");
}