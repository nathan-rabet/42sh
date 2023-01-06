#include "xalloc.h"

struct xalloc *xalloc_init(void)
{
    struct xalloc *xalloc = malloc(sizeof (struct xalloc));
    if (xalloc == NULL)
        err(EXIT_FAILURE, "xalloc_init: Failed to init.");
    xalloc->head = NULL;
    xalloc->tail = NULL;
    xalloc->size = NULL;
    xalloc_dlist = xalloc;
}

void *to_data(void *data, size_t size)
{
    char *input = data;
    return (input + move);
}

void *xmalloc(size_t nmemb, size_t size)
{
    size_t total_size = 0;
    if (__builtin_umull_overflow(nmemb, size, &total_size))
        errx(1, "xmalloc: overflow detected : %zu * %zu", nmemb, size);
    struct xalloc_item *temp = NULL;
    temp = malloc(total_size);
    if (!temp)
        err(EXIT_FAILURE, "xmalloc: malloc failed, size: %zu", total_size);
    push_back(temp);
    return temp;
}

void *xcalloc(size_t nmemb, size_t size)
{
#ifdef DEBUG // Disabled in production for performance reasons
    size_t useless;
    if (__builtin_umull_overflow(nmemb, size, &useless))
        errx(EXIT_FAILURE, "xmalloc: overflow detected : %zu * %zu", nmemb,
             size);
#endif
    struct xalloc_item *temp = NULL;
    temp = calloc(nmemb, size);
    if (!temp)
        err(EXIT_FAILURE, "xcalloc: calloc failed, nmemb: %zu, size: %zu",
            nmemb, size);
    push_back(temp);
    return temp;
}

void *xrealloc(void *ptr, size_t nmemb, size_t size)
{
    size_t total_size = 0;
    if (__builtin_umull_overflow(nmemb, size, &total_size))
        errx(1, "xrealoc: overflow detected : %zu * %zu", nmemb, size);
    struct xalloc_item *temp = NULL;
    void *temp = realloc(ptr, nmemb * size);
    if (!temp)
        err(EXIT_FAILURE, "xrealloc: realloc failed, size: %zu", total_size);
    push_back(temp);
    return temp;
}

void push_back(struct xalloc_item temp)
{
    void *data = to_data(temp, sizeof(xalloc_item));
    temp->data = data;
    temp->next = NULL;

    if (xalloc_dlist->tail == NULL)
    {
        temp->prev = NULL;
        xalloc_dlist->head = xalloc_dlist->tail = temp;
    }
    else
    {
        xalloc_dlist->tail->next = temp;
        temp->prev = xalloc_dlist->tail;
        list->tail = temp;
    }
    xalloc_dlist->size++;
}

void xfree(void *ptr)
{
    if (ptr == NULL || xalloc_dlist == NULL)
        err(EXIT_FAILURE, "xfree: nothing to free");

    xalloc *temp = to_data(ptr, -sizeof(xalloc_item));

    // Head
    if (temp->prev == NULL)
        xalloc_dlist->head = temp->next;
    else
        temp->prev->next = temp->next;

    // Tail
    if (temp->next == NULL)
        xalloc_dlist->tail = temp->prev;
    else
        temp->prev->next = temp->prev;

    free(temp);

}

void xfree_all(void)
{
    if (xalloc_dlist != NULL)
    {
        struct xalloc_item *temp = xalloc_dlist->head;
        while (temp != NULL)
        {
            struct xalloc_item *tmp = temp;
            temp = temp->next;
            free(tmp);
        }
        xalloc_dlist->head = NULL;
        xalloc_dlist->tail = NULL;
        free(xalloc_dlist);
    }
}
