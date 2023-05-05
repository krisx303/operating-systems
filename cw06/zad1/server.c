#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include "lib.h"

// global variables
int clients[MAX_CLIENTS] = {0};
int serverQueue;

void signal_handler(int signo)
{
    Message message;
    message.type = TYPE_STOP;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] == 0)
            continue;
        sendQueueMessage(clients[i], &message);
    }
    closeQueue(serverQueue);
    exit(EXIT_SUCCESS);
}

void addDateToMessage(Message *message)
{
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    snprintf(message->date, MESSAGE_TEXT_SIZE, "%02d:%02d:%02d", time.tm_hour, time.tm_min, time.tm_sec);
}

void saveLogToFile(int type, Message *message)
{
    FILE *fileLog = fopen("log.log", "a");

    fprintf(fileLog, "[%s] command %ld from %d\n", message->date, message->type, message->from);
    fprintf(stdout, "[%s] command %ld from %d\n", message->date, message->type, message->from);

    fclose(fileLog);
}

int main(int argc, const char *argv[])
{
    serverQueue = openQueue(SERVER_ID);
    if (serverQueue == -1)
        exit(EXIT_FAILURE);

    Message message;
    int type = -1;
    int clientID = 1;

    signal(SIGINT, signal_handler);

    while (1)
    {
        receiveQueueMessage(serverQueue, &message, TYPE_PRIORITY);
        addDateToMessage(&message);
        saveLogToFile(type, &message);
        type = message.type;
        switch (type)
        {
        case TYPE_INIT:
            int clientQueue = message.content.number;
            if (clientID == MAX_CLIENTS)
            {
                message.content.number = -1;
            }
            else
            {
                clients[clientID] = clientQueue;
                message.content.number = clientID;
                clientID++;
            }
            sendQueueMessage(clientQueue, &message);
            break;

        case TYPE_STOP:
            int client = message.content.number;
            clients[client] = 0;
            break;

        case TYPE_LIST:
            int id = message.content.number;
            char content[MESSAGE_TEXT_SIZE];
            int k = 0;
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if (clients[i] == 0)
                    continue;
                message.content.text[k++] = i;
            }
            message.content.text[k] = '\0';
            message.type = TYPE_LIST;

            sendQueueMessage(clients[id], &message);
            break;

        case TYPE_TO_ALL:
            message.type = TYPE_TO_ONE;
            for (int i = 0; i < clientID; i++)
            {
                if (clients[i] == 0 || i == message.from)
                    continue;
                sendQueueMessage(clients[i], &message);
            }
            break;

        case TYPE_TO_ONE:
            if (message.to >= MAX_CLIENTS || clients[message.to] == 0)
                break;
            sendQueueMessage(clients[message.to], &message);
            break;

        default:
            break;
        }
    }

    if (closeQueue(serverQueue) == -1)
        exit(EXIT_FAILURE);

    printf("Server has ended\n");
    return EXIT_SUCCESS;
}