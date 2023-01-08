#include "xalloc.h"

#ifndef DEBUG
static
#endif
    struct xalloc xalloc_dlist = { .initialized = false,
                                   .mutex = PTHREAD_MUTEX_INITIALIZER,
                                   .head = NULL,
                                   .tail = NULL };

#define LOCK_MUTEX()                                                           \
    if (pthread_mutex_lock(&xalloc_dlist.mutex) != 0)                          \
    err(EXIT_FAILURE, "LOCK_MUTEX: Failed to lock mutex.")

#define UNLOCK_MUTEX()                                                         \
    if (pthread_mutex_unlock(&xalloc_dlist.mutex) != 0)                        \
    err(EXIT_FAILURE, "UNLOCK_MUTEX: Failed to unlock mutex.")

#define CHECK_INITIALIZED(fun_name)                                            \
    if (xalloc_dlist.initialized == false)                                     \
        errx(EXIT_FAILURE, fun_name ": xalloc_dlist not initialized");

void xalloc_init(void)
{
    xalloc_dlist.head = NULL;
    xalloc_dlist.tail = NULL;

    // Initialize linked-list mutex
    if (pthread_mutex_init(&xalloc_dlist.mutex, NULL) != 0)
        err(EXIT_FAILURE, "xalloc_init: Failed to initialize mutex.");
}

void xalloc_deinit(void)
{
    CHECK_INITIALIZED("xalloc_deinit");

    LOCK_MUTEX();

    xfree_all();

    // Delete mutex
    if (pthread_mutex_destroy(&xalloc_dlist.mutex) != 0)
        err(EXIT_FAILURE, "xalloc_init: Failed to destroy mutex.");
}

/**
 * @brief push the item into the linked list
 * @param ptr the item to push
 */
static void push_back(void *ptr)
{
    struct xalloc_item *item = malloc(sizeof(struct xalloc_item));
    if (item == NULL)
        err(EXIT_FAILURE, "push_back: Failed to push back.");

    item->ptr = ptr;
    item->next = NULL;

    LOCK_MUTEX();
    if (xalloc_dlist.tail == NULL) // || xalloc_dlist.head == NULL)
    {
        item->prev = NULL;
        xalloc_dlist.head = xalloc_dlist.tail = item;
    }
    else
    {
        xalloc_dlist.tail->next = item;
        item->prev = xalloc_dlist.tail;
        xalloc_dlist.tail = item;
    }

    UNLOCK_MUTEX();
}

/**
 * @brief Delete the item from the linked list
 * @param ptr the item to delete
 */
static bool delete_ptr(void *ptr)
{
    LOCK_MUTEX();

    struct xalloc_item *item = xalloc_dlist.head;
    while (item != NULL)
    {
        if (item->ptr == ptr)
        {
            if (item->prev == NULL)
            {
                xalloc_dlist.head = item->next;
                if (item->next != NULL)
                    item->next->prev = NULL;
            }
            else if (item->next == NULL)
            {
                xalloc_dlist.tail = item->prev;
                if (item->prev != NULL)
                    item->prev->next = NULL;
            }
            else
            {
                item->prev->next = item->next;
                item->next->prev = item->prev;
            }
            free(item);
            UNLOCK_MUTEX();
            return true;
        }
        item = item->next;
    }
    UNLOCK_MUTEX();
    return false;
}

void *xmalloc(size_t nmemb, size_t size)
{
    CHECK_INITIALIZED("xmalloc");
    size_t total_size = 0;
    if (__builtin_umull_overflow(nmemb, size, &total_size))
        errx(1, "xmalloc: overflow detected : %zu * %zu", nmemb, size);
    struct xalloc_item *ptr = malloc(total_size);
    if (!ptr)
        err(EXIT_FAILURE, "xmalloc: malloc failed, size: %zu", total_size);
    push_back(ptr);
    return ptr;
}

void *xcalloc(size_t nmemb, size_t size)
{
    CHECK_INITIALIZED("xcalloc");
#ifdef DEBUG // Disabled in production for performance reasons
    size_t useless;
    if (__builtin_umull_overflow(nmemb, size, &useless))
        errx(EXIT_FAILURE, "xmalloc: overflow detected : %zu * %zu", nmemb,
             size);
#endif
    void *ptr = calloc(nmemb, size);
    if (!ptr)
        err(EXIT_FAILURE, "xcalloc: calloc failed, nmemb: %zu, size: %zu",
            nmemb, size);
    push_back(ptr);
    return ptr;
}

void *xrealloc(void *ptr, size_t nmemb, size_t size)
{
    CHECK_INITIALIZED("xrealloc");
    size_t total_size = 0;
    if (__builtin_umull_overflow(nmemb, size, &total_size))
        errx(1, "xrealoc: overflow detected : %zu * %zu", nmemb, size);
    struct xalloc_item *new_ptr = realloc(ptr, total_size);
    if (!new_ptr)
        err(EXIT_FAILURE, "xrealloc: realloc failed, size: %zu", total_size);
    if (!delete_ptr(ptr))
        errx(EXIT_FAILURE, "xrealloc: pointer '%p' not found in linked-list",
             ptr);
    push_back(new_ptr);
    return new_ptr;
}

void xfree(void *ptr)
{
    CHECK_INITIALIZED("xfree");
    if (ptr != NULL)
        return;

    if (!delete_ptr(ptr))
        errx(EXIT_FAILURE, "xfree: pointer '%p' not found in linked-list", ptr);
    free(ptr);
}

void xfree_all(void)
{
    CHECK_INITIALIZED("xfree_all");
    LOCK_MUTEX();
    struct xalloc_item *runner = xalloc_dlist.head;
    while (runner != NULL)
    {
        struct xalloc_item *tmp = runner;
        free(runner->ptr);
        runner = runner->next;
        free(tmp);
    }
    xalloc_dlist.head = NULL;
    xalloc_dlist.tail = NULL;
    UNLOCK_MUTEX();
}
