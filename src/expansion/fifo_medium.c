#include <stdlib.h>

#include "evalexpr.h"

// Pushes element and token to front.
void fifo_push(struct fifo *fifo, int elt, char token)
{
    // Create new element.
    struct list *element = xmalloc(1, sizeof(struct list));
    element->data = elt;
    element->token = token;
    element->next = NULL;

    // Handle case for empty queue.
    if (fifo->tail == NULL)
    {
        fifo->head = element;
        fifo->tail = element;
    }
    else
    {
        fifo->tail->next = element;
        fifo->tail = element;
    }

    fifo->size += 1;
}

// Returns head of queue.
int fifo_head(struct fifo *fifo)
{
    return fifo->head->data;
}

// Removes head of queue.
void fifo_pop(struct fifo *fifo)
{
    // Check for empty queue.
    if (fifo->size == 0)
        return;

    // Save pointer copy of next element.
    struct list *next = fifo->head->next;

    // Free head.
    xfree(fifo->head);
    fifo->size -= 1;

    // Set tail to NULL if queue empty.
    if (fifo->size == 0)
        fifo->tail = NULL;

    // Move head to next element.
    fifo->head = next;
}
