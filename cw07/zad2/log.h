#pragma once

#include <unistd.h>
#include <time.h>

#define DEBUG "DEB"
#define ERROR "ERR"
#define WAR "WAR"

void printLog(const char *type, const char *name, const char *message)
{
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    printf("%02d:%02d:%02d [%s] [%12s] %s", time.tm_hour, time.tm_min, time.tm_sec, type, name, message);
}

void printLogID(const char *type, const char *name, int id, const char *message)
{
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    printf("%02d:%02d:%02d [%s] [%9s-%02d] %s", time.tm_hour, time.tm_min, time.tm_sec, type, name, id, message);
}

void printDebug(const char *name, const char *message)
{
    printLog(DEBUG, name, message);
}

void printDebugID(const char *name, int id, const char *message)
{
    printLogID(DEBUG, name, id, message);
}

void printError(const char *name, const char *message)
{
    printLog(ERROR, name, message);
}

void printErrorID(const char *name, int id, const char *message)
{
    printLogID(ERROR, name, id, message);
}