#include "../../include/execution.h"

struct list_dup list_dup = {
        .head = NULL,
};

void push_top_dup(int fd_source, int fd_destination)
{
    struct dup_item *item = xmalloc(1, sizeof (struct dup_item));
    item->fd_source = fd_source;
    item->fd_dest = fd_destination;
    item->fd_source_dup = -1;
    item->next = NULL;
    if (list_dup.head == NULL)
        list_dup.head = item;
    else
    {
        item->next = list_dup.head;
        list_dup.head = item;
    }
}

void dup2_all_list()
{
    if (list_dup.head == NULL)
        return;
    struct dup_item *item = list_dup.head;
    while (item != NULL)
    {
        item->fd_source_dup = dup(item->fd_source);

        if (dup2(item->fd_source, item->fd_dest) == -1)
            return_and_free("Dup2 failed", 1);
        item = item->next;
    }
}

bool is_std(int IO)
{
    switch (IO) {
        case 1:
        case 2:
        case 3:
            return true;
        default:
            return false;
    }
}

void close_all_list_dup()
{
    if (list_dup.head == NULL)
        return;
    struct dup_item *item = list_dup.head;
    while (item != NULL)
    {
        dup2(item->fd_source_dup, item->fd_source);

        if (!is_std(item->fd_source))
            close(item->fd_source);
        if (!is_std(item->fd_dest))
            close(item->fd_dest);
        item = item->next;
    }
}