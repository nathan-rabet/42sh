#ifndef XALLOC_H
#define XALLOC_H

#include <err.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/**
 * @brief Structure of the memory allocation
 * @param ptr the data pointer returned by malloc, calloc or realloc
 * @param next pointer to the next block of memory
 * @param prev pointer to the previous block of memory
 */
struct xalloc_item
{
    void *ptr;
    struct xalloc_item *next;
    struct xalloc_item *prev;
};

/**
 * @brief The doubled linked list
 * @param size The size of the double linked list
 * @param head The first block of the linked list
 * @param tail The last block of the linked list
 */
struct xalloc
{
    bool initialized;
    pthread_mutex_t mutex;
    struct xalloc_item *head;
    struct xalloc_item *tail;
};

/**
 * @brief Initialize the xalloc linked list
 * (nullify the head and tail and create the mutex)
 */
void xalloc_init(void);

/**
 * @brief Deinitialize the xalloc linked list
 * (free all the pointers and delete the mutex)
 */
void xalloc_deinit(void);

/**
 * @brief Malloc wrapper that exits the program if the allocation fails
 * or if nmemb * size overflows.
 * It also save the returned pointer in a list of pointers to free when the
 * program exits.
 *
 * @param nmemb Number of elements to allocate
 * @param size Size of each element
 * @return void*
 */
void *xmalloc(size_t nmemb, size_t size);

/**
 * @brief Calloc wrapper that exits the program if the allocation fails
 * or if nmemb * size overflows.
 * It also save the returned pointer in a list of pointers to free when
 * the program exits.
 *
 * @param nmemb Number of elements to allocate
 * @param size Size of each element
 * @return void*
 */
void *xcalloc(size_t nmemb, size_t size);

/**
 * @brief Realloc wrapper that exits the program if the allocation fails
 * or if nmemb * size overflows.
 * It also save the returned pointer in a list of pointers to free when
 * the program exits.
 *
 * @param ptr Pointer to the memory to reallocate
 * @param nmemb Number of elements to allocate
 * @param size Size of each element
 * @return void*
 */
void *xrealloc(void *ptr, size_t nmemb, size_t size);

/**
 * @brief Free a pointer allocated by xmalloc, xcalloc or xrealloc.
 * It also remove the pointer from the list of pointers.
 *
 * @param ptr Pointer to free
 */
void xfree(void *ptr);

/**
 * @brief Free all the pointers saved by xmalloc, xcalloc and xrealloc.
 */
void xfree_all(void);

#endif /* XALLOC_H */
