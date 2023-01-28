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
 */
struct xalloc
{
    bool initialized; // true if the list is initialized, false otherwise
    pthread_mutex_t mutex; // mutex to lock the list
    struct xalloc_item *head; // head of the list
};

/**
 * @brief Initialize the xalloc linked list
 * (nullify the head and create the mutex)
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

// XALLOC UTILS

/**
 * @brief Strdup wrapper which uses xalloc functions.
 *
 * @warning Must be used instead of `strdup` in case you are using xalloc,
 * else you will have memory leaks.
 *
 * @param s String to duplicate
 * @return char*
 */
char *xstrdup(const char *s);

#endif /* XALLOC_H */
