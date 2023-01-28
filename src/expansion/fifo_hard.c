#include <stdio.h>
#include <stdlib.h>

#include "evalexpr.h"

// Removes all elements of queue.
void fifo_clear(struct fifo *fifo)
{
    while (fifo->size)
        fifo_pop(fifo);
}

// Destroys queue.
void fifo_destroy(struct fifo *fifo)
{
    fifo_clear(fifo);
    xfree(fifo);
}

// Prints elements of queue.
void fifo_print(const struct fifo *fifo)
{
    struct list *curr = fifo->head;

    while (curr != NULL)
    {
        printf("%d\n", curr->data);

        curr = curr->next;
    }
}
