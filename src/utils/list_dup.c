#include "../include/execution.h"

struct list_dup list_dup = {
        .head = NULL,
};

static void push_top_dup(int fd)
{
    struct dup_item *item = xmalloc(1, sizeof (struct dup_item));
    item->fd = fd;
    if (list_dup.head == NULL)
        list_dup.head = item;
    else
    {
        item->next = list_dup.head;
        list_dup.head = item;
    }
}