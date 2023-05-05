#pragma once
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define MESSAGE_TEXT_SIZE 128
#define MESSAGE_CONTENT_SIZE sizeof(Message)
#define SERVER_ID 13
#define TYPE_PRIORITY -6 // for receiving messages with priorities
#define MAX_CLIENTS 32

/** Message type enum */
typedef enum MessageType
{
    TYPE_STOP = 1,
    TYPE_LIST = 2,
    TYPE_TO_ONE = 3,
    TYPE_TO_ALL = 4,
    TYPE_INIT = 5
} MessageType;

typedef union MessageContent
{
    char text[MESSAGE_TEXT_SIZE];
    int number;
} MessageContent;

/** New queue message structure */
typedef struct Message
{
    long type;
    int from;
    int to;
    char date[MESSAGE_TEXT_SIZE];
    MessageContent content;
} Message;

/** Returns path like: /home/xyz/ */
const char *getHomeDir()
{
    struct passwd *pw = getpwuid(getuid());
    return pw->pw_dir;
}

key_t generateUniqueKey(int id)
{
    return ftok(getHomeDir(), id);
}

/** Opens (or creates new) message queue */
int openQueue(int id)
{
    key_t key = generateUniqueKey(id);

    // Create new message queue
    int serverQueue = msgget(key, IPC_CREAT | 0600);
    if (serverQueue == -1)
        perror("Failed to open message queue");
    return serverQueue;
}

/** Closes message queue with given ID */
int closeQueue(int queueID)
{
    int out = msgctl(queueID, IPC_RMID, NULL);
    if (out == -1)
        perror("Failed to remove message queue");
    return out;
}

/** Sends message to given queue with Message content  */
int sendQueueMessage(int queueID, Message *content)
{
    int out = msgsnd(queueID, content, MESSAGE_CONTENT_SIZE, 0);
    if (out == -1)
        perror("Failed to send message");
    return out;
}
/** Receives message from given queue and put message into buffer */
int receiveQueueMessage(int queueID, Message *buffer, int priority)
{
    int out = msgrcv(queueID, buffer, MESSAGE_CONTENT_SIZE, priority, 0);
    if (out == -1)
        perror("Error receiving message");
    return out;
}
