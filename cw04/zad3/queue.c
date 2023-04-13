#include <stdlib.h>

typedef struct QueueNode
{
    struct QueueNode *next;
    int value;
} QueueNode;

typedef struct Queue
{
    QueueNode *head;
    QueueNode *tail;
    int size;
} Queue;

Queue *initQueue()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->head = NULL;
    queue->size = 0;
    queue->tail = NULL;
    return queue;
}

QueueNode *initQueueNode(int value)
{
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    node->next = NULL;
    node->value = value;
    return node;
}

void pushQueue(Queue *queue, int value)
{
    QueueNode *new_node = initQueueNode(value);
    if (queue->size > 0)
        queue->head->next = new_node;
    if (queue->size == 0)
        queue->tail = new_node;
    queue->head = new_node;
    queue->size++;
}

int popQueue(Queue *queue)
{
    QueueNode *tail = queue->tail;
    int output = tail->value;
    queue->tail = tail->next;
    free(tail);
    queue->size--;
    return output;
}

void printQueue(Queue *queue)
{
    QueueNode *node = queue->tail;
    for (int i = 0; i < queue->size; i++)
    {
        printf("%d -> ", node->value);
        node = node->next;
    }
    printf("NULL\n");
}

void destroyQueue(Queue *queue)
{
    QueueNode *node = queue->head, *prev;
    for (int i = 0; i < queue->size; i++)
    {
        prev = node;
        node = node->next;
        free(prev);
    }
}