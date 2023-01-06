#ifndef XALLOC_H
#define XALLOC_H

#include <err.h>
#include <stddef.h>
#include <stdlib.h>

struct xalloc_item
{
    void *ptr;
    struct xalloc_item *next;
    struct xalloc_item *prev;
};

struct xalloc
{
    size_t size;
    struct xalloc_item *head;
    struct xalloc_item *tail;
};

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
