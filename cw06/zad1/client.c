#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "lib.h"

// global variables

int cID;
int serverQ, clientQ;

/** Enum for entered commands by user */
typedef enum Command
{
    COMMAND_LIST,
    COMMAND_2ALL,
    COMMAND_2ONE,
    COMMAND_STOP,
    COMMAND_INVALID,
    COMMAND_HELP
} Command;

/** Sends message to server with client queue ID */
int sendInitialMessage(int serverQueue, int clientQ)
{
    Message message;
    message.type = TYPE_INIT;
    message.from = -1;
    message.content.number = clientQ;

    return sendQueueMessage(serverQueue, &message);
}

/** Receives client ID */
int receiveClientID(int clientQueue)
{
    Message message;
    receiveQueueMessage(clientQueue, &message, TYPE_INIT);
    return message.content.number;
}

int startsWith(const char *str, const char *prefix)
{
    size_t len_prefix = strlen(prefix);
    return strncmp(str, prefix, len_prefix) == 0;
}

/** Reads user input and parse it into Command */
Command readUserCommand(char *buff)
{
    fgets(buff, MESSAGE_TEXT_SIZE, stdin);
    buff[strlen(buff) - 1] = '\0';
    if (!strcmp(buff, "LIST"))
        return COMMAND_LIST;
    if (!strcmp(buff, "STOP"))
        return COMMAND_STOP;
    if (!strcmp(buff, "HELP"))
        return COMMAND_HELP;
    if (startsWith(buff, "2ALL "))
        return COMMAND_2ALL;
    if (startsWith(buff, "2ONE "))
        return COMMAND_2ONE;
    return COMMAND_INVALID;
}

/** Prints all possible user commands */
void printHelp()
{
    printf("Help commands:\n");
    printf("'LIST' -> lists all connected user to this server\n");
    printf("'STOP' -> stops this program\n");
    printf("'2ALL $string' -> sends given string to all connected users\n");
    printf("'2ONE #id $string' -> sends given string to user with that id\n");
    printf("'HELP' -> this command ;)\n");
}

/** Sends request message with different type and clientID as content*/
int sendServerMessage(int serverQueue, int type, int clientID)
{
    Message message;
    message.type = type;
    message.content.number = clientID;
    message.from = clientID;

    return sendQueueMessage(serverQueue, &message);
}

/** Receives list of users */
void receiveListMessage(int clientQueue, int clientID)
{
    Message message;
    receiveQueueMessage(clientQueue, &message, TYPE_LIST);
    printf("Active users:\n");
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (message.content.text[i] == '\0')
            break;
        printf("User with ID: %d\n", message.content.text[i]);
    }
}

/** Signal handler  */
void signal_handler(int signo)
{
    sendServerMessage(serverQ, TYPE_STOP, cID);
    printf("\nProgram is ending...\n");
    if (closeQueue(clientQ) == -1)
        exit(EXIT_FAILURE);
    exit(EXIT_SUCCESS);
}

int sendStringMessageToAll(int serverQueue, char *buff, int clientID)
{
    Message message;
    message.type = TYPE_TO_ALL;
    message.from = clientID;
    strcpy(message.content.text, buff + sizeof(char) * 5);

    return sendQueueMessage(serverQueue, &message);
}

int sendStringMessageToOne(int serverQueue, char *buff, int to, int clientID)
{
    Message message;
    message.type = TYPE_TO_ONE;
    message.from = clientID;
    message.to = to;
    strcpy(message.content.text, buff + sizeof(char) * 5);

    return sendQueueMessage(serverQueue, &message);
}

void readWaitedMessages()
{
    Message message;
    while (msgrcv(clientQ, &message, MESSAGE_CONTENT_SIZE, 0, IPC_NOWAIT) >= 0)
    {
        if (message.type == TYPE_STOP)
        {
            printf("\nServer stopped working...\n");
            if (closeQueue(clientQ) == -1)
                exit(EXIT_FAILURE);
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Received message from %d at %s\n%s\n", message.from, message.date, message.content.text);
        }
    }
}

int findFirstSpace(char *arr)
{
    int i;
    for (i = 0; i < strlen(arr); i++)
    {
        if (arr[i] == ' ')
            return i + 1;
    }
    return i + 1;
}

int main(int argc, const char *argv[])
{
    int serverQueue = openQueue(SERVER_ID);
    if (serverQueue == -1)
        exit(EXIT_FAILURE);

    int clientQueue = openQueue(getpid());
    if (clientQueue == -1)
        exit(EXIT_FAILURE);

    if (sendInitialMessage(serverQueue, clientQueue) == -1)
        exit(EXIT_FAILURE);

    int clientID = receiveClientID(clientQueue);
    if (clientID == -1)
    {
        printf("Server is overloaded!\n");
        if (closeQueue(clientQueue) == -1)
            exit(EXIT_FAILURE);
        exit(EXIT_SUCCESS);
    }
    // setting global variables
    cID = clientID;
    serverQ = serverQueue;
    clientQ = clientQueue;

    signal(SIGINT, signal_handler);

    printf("received client id: %d\n", clientID);

    Command command = 0;
    char buff[MESSAGE_TEXT_SIZE];

    while (command != COMMAND_STOP)
    {
        printf("Enter command: ");
        command = readUserCommand(buff);

        readWaitedMessages();
        switch (command)
        {
        case COMMAND_STOP:
            sendServerMessage(serverQueue, TYPE_STOP, clientID);
            printf("Program is ending...\n");
            break;

        case COMMAND_HELP:
            printHelp();
            break;

        case COMMAND_LIST:
            sendServerMessage(serverQueue, TYPE_LIST, clientID);
            receiveListMessage(clientQueue, clientID);
            break;

        case COMMAND_2ALL:
            sendStringMessageToAll(serverQueue, buff, clientID);
            break;

        case COMMAND_2ONE:
            int space = findFirstSpace(buff + 5 * sizeof(char));
            int to = atoi(buff + 5 * sizeof(char));
            sendStringMessageToOne(serverQueue, buff + sizeof(char) * space, to, clientID);
            break;

        case COMMAND_INVALID:
            printf("Invalid command (you may enter help for command list)\n");
            break;

        default:
            break;
        }
    }

    if (closeQueue(clientQueue) == -1)
        exit(EXIT_FAILURE);

    return EXIT_SUCCESS;
}