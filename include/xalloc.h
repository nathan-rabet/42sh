#ifndef XALLOC_H
#define XALLOC_H

#include <err.h>
#include <stddef.h>
#include <stdlib.h>

extern struct xalloc *xalloc_dlist;

/**
 * @brief Structure of the memory allocation
 * @param data the data pointer
 * @param next pointer to the next block of memory
 * @param prev pointer to the previous block of memory
 */
struct xalloc_item
{
    void *data;
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
    size_t size;
    struct xalloc_item *head;
    struct xalloc_item *tail;
};


/**
 * @return the initialize dlist of pointer
 */
struct xalloc *xalloc_init(void);

/**
 * @brief change the location of the pointer
 * @param data the pointer to convert
 * @param size the distance
 * @return the pointer to the new location
 */
void *to_data(void *data, size_t size);

/**
 * @brief push the item into the stack
 * @param temp the item to push
 */
void push_back(struct xalloc_item *temp);

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
