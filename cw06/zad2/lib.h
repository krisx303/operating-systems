#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <mqueue.h>
#include <pwd.h>

#define MESSAGE_TEXT_SIZE 128
#define MESSAGE_CONTENT_SIZE sizeof(Message)
#define TYPE_PRIORITY -6 // for receiving messages with priorities
#define MAX_CLIENTS 10
#define SERVER_QUEUE_NAME "/squeue"

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

mqd_t openQueue(const char *name)
{
    struct mq_attr attr;
    attr.mq_msgsize = MESSAGE_CONTENT_SIZE;
    attr.mq_maxmsg = MAX_CLIENTS;
    int out = mq_open(name, O_RDWR | O_CREAT | O_EXCL, 0666, &attr);
    if (out == -1)
        perror("Unable to open queue");
    return out;
}

void randomName(char *name)
{
    name[0] = '/';
    for (int i = 1; i < 10; i++)
    {
        name[i] = rand() % ('z' - 'a' + 1) + 'a';
    }
}

/** Sends message to given queue with Message content  */
int sendQueueMessageSecure(char *name, Message *content)
{
    mqd_t que = mq_open(name, O_WRONLY);
    if (que == -1)
    {
        perror("Failed to open queue to send message");
        return que;
    }
    int out = mq_send(que, (char *)content, MESSAGE_CONTENT_SIZE, 0);
    if (out == -1)
    {
        perror("Failed to send message");
        return out;
    }
    out = mq_close(que);
    return out;
}

/** Sends message to given queue with Message content  */
int sendQueueMessage(int queueID, Message *content)
{
    int out = mq_send(queueID, (char *)content, MESSAGE_CONTENT_SIZE, 0);
    if (out == -1)
        perror("Failed to send message");
    return out;
}

/** Receives message from given queue and put message into buffer */
int receiveQueueMessage(int queueID, Message *buffer, int priority)
{
    int out = mq_receive(queueID, (char *)buffer, MESSAGE_CONTENT_SIZE, NULL);
    if (out == -1)
        perror("Error receiving message");
    return out;
}