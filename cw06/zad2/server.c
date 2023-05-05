#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <mqueue.h>
#include "lib.h"

// global variables
char *clients[MAX_CLIENTS];
mqd_t serverQueue;

void signal_handler(int signo)
{
    Message message;
    message.type = TYPE_STOP;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] == 0)
            continue;
        sendQueueMessageSecure(clients[i], &message);
        free(clients[i]);
    }
    mq_close(serverQueue);
    exit(EXIT_SUCCESS);
}

void addDateToMessage(Message *message)
{
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    snprintf(message->date, MESSAGE_TEXT_SIZE, "%02d:%02d:%02d", time.tm_hour, time.tm_min, time.tm_sec);
}

void saveLogToFile(Message *message)
{
    FILE *fileLog = fopen("log.log", "a");

    fprintf(fileLog, "[%s] command %ld from %d\n", message->date, message->type, message->from);
    fprintf(stdout, "[%s] command %ld from %d\n", message->date, message->type, message->from);

    fclose(fileLog);
}

int main(int argc, const char *argv[])
{
    // Remove any existing queue with the same name
    mq_unlink(SERVER_QUEUE_NAME);

    // Open a new message queue with the given name
    mqd_t serverQueue = openQueue(SERVER_QUEUE_NAME);
    if (serverQueue == -1)
        exit(EXIT_FAILURE);

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        clients[i] = 0;
    }

    Message message;
    int clientID = 1;

    signal(SIGINT, signal_handler);
    mqd_t que;

    while (1)
    {
        mq_receive(serverQueue, (char *)&message, MESSAGE_CONTENT_SIZE, NULL);
        addDateToMessage(&message);
        saveLogToFile(&message);
        switch (message.type)
        {
        case TYPE_INIT:
            if (clientID == MAX_CLIENTS)
            {
                message.content.number = -1;
            }
            else
            {
                clients[clientID] = calloc(10, sizeof(char));
                strncpy(clients[clientID], message.content.text, 10);
                message.content.number = clientID;
            }
            sendQueueMessageSecure(clients[clientID], &message);
            if (clientID != MAX_CLIENTS)
                clientID++;
            break;

        case TYPE_STOP:
            int client = message.content.number;
            free(clients[client]);
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

            sendQueueMessageSecure(clients[id], &message);
            break;

        case TYPE_TO_ALL:
            message.type = TYPE_TO_ONE;
            for (int i = 0; i < clientID; i++)
            {
                if (clients[i] == 0 || i == message.from)
                    continue;
                sendQueueMessageSecure(clients[i], &message);
            }
            break;

        case TYPE_TO_ONE:
            if (message.to >= MAX_CLIENTS || clients[message.to] == 0)
                break;
            sendQueueMessageSecure(clients[message.to], &message);
            break;

        default:
            break;
        }
    }

    // Close the message queue when done
    if (mq_close(serverQueue) == -1)
    {
        perror("Failed to close server queue");
        exit(EXIT_FAILURE);
    }

    return 0;
}