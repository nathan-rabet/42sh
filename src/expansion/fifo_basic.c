#include <stdlib.h>

#include "evalexpr.h"

// Returns a new queue.
struct fifo *fifo_init(void)
{
    struct fifo *fifo = xmalloc(1, sizeof(struct fifo));
    fifo->head = NULL;
    fifo->tail = NULL;
    fifo->size = 0;

    return fifo;
}

// Returns size of queue.
size_t fifo_size(struct fifo *fifo)
{
    return fifo->size;
}
