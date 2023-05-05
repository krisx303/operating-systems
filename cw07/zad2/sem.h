#pragma once

#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>

struct sembuf operationDown = {0, -1, 0};
struct sembuf operationUp = {0, 1, 0};

sem_t *semaphoreCreate(const char *fname, int initial)
{
    sem_t *semid = sem_open(fname, O_CREAT | O_EXCL, 0666, initial);
    if (semid == SEM_FAILED)
        return (sem_t *)-1;
    return semid;
}

sem_t *semaphoreOpen(const char *fname)
{
    sem_t *semid = sem_open(fname, 0);
    if (semid == SEM_FAILED)
        return (sem_t *)-1;
    return semid;
}

void semaphoreClose(sem_t *semid)
{
    sem_close(semid);
}

void semaphoreUnlink(const char *fname)
{
    sem_unlink(fname);
}

void semaphoreDown(sem_t *semid)
{
    sem_wait(semid);
}

void semaphoreUp(sem_t *semid)
{
    sem_post(semid);
}