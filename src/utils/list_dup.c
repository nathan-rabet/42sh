#include "../../include/execution.h"

struct list_dup list_dup = {
        .head = NULL,
};

static void push_top_dup(int fd, int fd_dup)
{
    struct dup_item *item = xmalloc(1, sizeof (struct dup_item));
    item->fd = fd;
    item->fd_dup = fd_dup;

    if (list_dup.head == NULL)
        list_dup.head = item;
    else
    {
        item->next = list_dup.head;
        list_dup.head = item;
    }
}