#include <stdlib.h>
#include "pathfinding.h"

/**
 * queue_create - Create a new empty queue
 *
 * Return: Pointer to the new queue, or NULL on failure
 */
queue_t *queue_create(void)
{
    queue_t *queue = malloc(sizeof(queue_t));

    if (!queue)
        return (NULL);

    queue->front = NULL;
    queue->back = NULL;
    return (queue);
}

/**
 * enqueue - Add an element to the back of the queue
 * @queue: Pointer to the queue
 * @content: Pointer to the data to enqueue
 *
 * Return: 1 on success, 0 on failure
 */
int enqueue(queue_t *queue, void *content)
{
    queue_node_t *node;

    if (!queue)
        return (0);

    node = malloc(sizeof(queue_node_t));
    if (!node)
        return (0);

    node->content = content;
    node->next = NULL;

    if (!queue->back)
    {
        queue->front = node;
        queue->back = node;
    }
    else
    {
        queue->back->next = node;
        queue->back = node;
    }

    return (1);
}

/**
 * dequeue - Remove and return the front element from the queue
 * @queue: Pointer to the queue
 *
 * Return: Pointer to the dequeued data, or NULL if queue is empty
 */
void *dequeue(queue_t *queue)
{
    queue_node_t *node;
    void *content;

    if (!queue || !queue->front)
        return (NULL);

    node = queue->front;
    content = node->content;
    queue->front = node->next;

    if (!queue->front)
        queue->back = NULL;

    free(node);
    return (content);
}

/**
 * queue_free - Free a queue without freeing its contents
 * @queue: Pointer to the queue
 */
void queue_free(queue_t *queue)
{
    if (!queue)
        return;

    while (queue->front)
        free(dequeue(queue));

    free(queue);
}
