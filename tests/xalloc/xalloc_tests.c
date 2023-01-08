#include <criterion/criterion.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "xalloc.h"

extern struct xalloc xalloc_dlist;

inline static void exec_in_fork(void (*func)(void))
{
    pid_t pid = fork();

    if (pid == 0) // Child process
        func();
    else if (pid > 0) // Parent process
    {
        if (wait(NULL) == -1)
            cr_assert_fail("Failed to wait for child process.");
    }
    else // Error
        cr_assert_fail("Failed to fork process.");
}

// -----------------------------------------------------------------------------
// Tests functions
// -----------------------------------------------------------------------------

static void xalloc_init_test(void)
{
    xalloc_init();
    cr_assert_eq(xalloc_dlist.initialized, true);
    xalloc_deinit();
}

static void not_xalloc_init_test(void)
{
    cr_assert_eq(xalloc_dlist.initialized, false);
}

static void xmalloc_test(void)
{
    xalloc_init();

    void *ptr = xmalloc(42, sizeof(int));
    cr_assert_not_null(ptr);
    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_not_null(xalloc_dlist.tail);
    cr_assert_null(xalloc_dlist.head->prev);
    cr_assert_null(xalloc_dlist.tail->next);
    cr_assert_eq(xalloc_dlist.head, xalloc_dlist.tail);
    cr_assert_eq(xalloc_dlist.head->ptr, ptr);
    cr_assert_eq(xalloc_dlist.tail->ptr, ptr);

    xfree(ptr);
    cr_assert_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.tail);

    xalloc_deinit();
}

static void xcalloc_test(void)
{
    xalloc_init();

    void *ptr = xcalloc(42, sizeof(int));
    cr_assert_not_null(ptr);
    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_not_null(xalloc_dlist.tail);
    cr_assert_null(xalloc_dlist.head->prev);
    cr_assert_null(xalloc_dlist.tail->next);
    cr_assert_eq(xalloc_dlist.head, xalloc_dlist.tail);
    cr_assert_eq(xalloc_dlist.head->ptr, ptr);
    cr_assert_eq(xalloc_dlist.tail->ptr, ptr);

    // Test if the memory is zeroed
    cr_expect_arr_eq(ptr, calloc(42, sizeof(int)), 42 * sizeof(int));

    xfree(ptr);
    cr_assert_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.tail);

    xalloc_deinit();
}

static void xrealloc_test(void)
{
    xalloc_init();

    void *ptr = xmalloc(42, sizeof(int));
    cr_assert_not_null(ptr);
    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_not_null(xalloc_dlist.tail);
    cr_assert_null(xalloc_dlist.head->prev);
    cr_assert_null(xalloc_dlist.tail->next);
    cr_assert_eq(xalloc_dlist.head, xalloc_dlist.tail);
    cr_assert_eq(xalloc_dlist.head->ptr, ptr);
    cr_assert_eq(xalloc_dlist.tail->ptr, ptr);

    ptr = xrealloc(ptr, 84, sizeof(int));
    cr_assert_not_null(ptr);
    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_not_null(xalloc_dlist.tail);
    cr_assert_null(xalloc_dlist.head->prev);
    cr_assert_null(xalloc_dlist.tail->next);
    cr_assert_eq(xalloc_dlist.head, xalloc_dlist.tail);
    cr_assert_eq(xalloc_dlist.head->ptr, ptr);
    cr_assert_eq(xalloc_dlist.tail->ptr, ptr);

    xfree(ptr);
    cr_assert_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.tail);

    xalloc_deinit();
}

static void xfree_test(void)
{
    xalloc_init();

    void *ptr = xmalloc(42, sizeof(int));
    cr_assert_not_null(ptr);
    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_not_null(xalloc_dlist.tail);
    cr_assert_null(xalloc_dlist.head->prev);
    cr_assert_null(xalloc_dlist.tail->next);
    cr_assert_eq(xalloc_dlist.head, xalloc_dlist.tail);
    cr_assert_eq(xalloc_dlist.head->ptr, ptr);
    cr_assert_eq(xalloc_dlist.tail->ptr, ptr);

    xfree(ptr);
    cr_assert_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.tail);

    xalloc_deinit();
}

static void xfree_null_test(void)
{
    xalloc_init();

    xfree(NULL);
    cr_assert_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.tail);

    xalloc_deinit();
}

static void xfree_not_found_test(void)
{
    xalloc_init();

    xfree((void *)0x42);
    // Must not reach this line
    cr_assert_fail();
}

static void xfree_double_free_test(void)
{
    xalloc_init();

    void *ptr = xmalloc(42, sizeof(int));
    cr_assert_not_null(ptr);
    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_not_null(xalloc_dlist.tail);
    cr_assert_null(xalloc_dlist.head->prev);
    cr_assert_null(xalloc_dlist.tail->next);
    cr_assert_eq(xalloc_dlist.head, xalloc_dlist.tail);
    cr_assert_eq(xalloc_dlist.head->ptr, ptr);
    cr_assert_eq(xalloc_dlist.tail->ptr, ptr);

    xfree(ptr);
    cr_assert_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.tail);

    xfree(ptr);
    // Must not reach this line
    cr_assert_fail();
}

static void xfree_all_test(void)
{
    xalloc_init();

    for (int i = 0; i < 42; i++)
    {
        xmalloc(42, sizeof(int));
    }

    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_not_null(xalloc_dlist.tail);
    cr_assert_null(xalloc_dlist.head->prev);
    cr_assert_null(xalloc_dlist.tail->next);
    cr_assert_neq(xalloc_dlist.head, xalloc_dlist.tail);

    xfree_all();
    cr_assert_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.tail);

    xalloc_deinit();
}

static void xfree_all_null_test(void)
{
    xalloc_init();

    xfree_all();
    cr_assert_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.tail);

    xalloc_deinit();
}

static void xfree_all_double_test(void)
{
    xalloc_init();

    for (int i = 0; i < 42; i++)
    {
        xmalloc(42, sizeof(int));
    }

    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_not_null(xalloc_dlist.tail);
    cr_assert_null(xalloc_dlist.head->prev);
    cr_assert_null(xalloc_dlist.tail->next);
    cr_assert_neq(xalloc_dlist.head, xalloc_dlist.tail);

    xfree_all();
    cr_assert_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.tail);

    xfree_all();
    // Must not reach this line
    cr_assert_fail();
}

static void *_xmalloc_multithreading_stress_thread(void *arg)
{
    const int nb_allocs = *((int *)arg);

    for (int i = 0; i < nb_allocs; i++)
    {
        void *ptr = xmalloc(42, sizeof(int));
        cr_assert_not_null(ptr);
        cr_assert_not_null(xalloc_dlist.head);
        cr_assert_not_null(xalloc_dlist.tail);
        cr_assert_null(xalloc_dlist.head->prev);
        cr_assert_null(xalloc_dlist.tail->next);
        cr_assert_neq(xalloc_dlist.head, xalloc_dlist.tail);

        xfree(ptr);
        cr_assert_null(xalloc_dlist.head);
        cr_assert_null(xalloc_dlist.tail);
    }

    return NULL;
}

static void xmalloc_multithreading_stress_test(void)
{
    xalloc_init();

    const int nb_threads = 42;
    const int nb_allocs = 42;
    pthread_t threads[42];

    for (int i = 0; i < nb_threads; i++)
        pthread_create(&threads[i], NULL, _xmalloc_multithreading_stress_thread,
                       (void *)&nb_allocs);

    for (int i = 0; i < nb_threads; i++)
        pthread_join(threads[i], NULL);

    cr_assert_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.tail);

    xalloc_deinit();
}

// -----------------------------------------------------------------------------
// Forked tests
// -----------------------------------------------------------------------------

Test(xalloc, xalloc_init)
{
    exec_in_fork(xalloc_init_test);
}

Test(xalloc, not_xalloc_init)
{
    exec_in_fork(not_xalloc_init_test);
}

Test(xalloc, xmalloc)
{
    exec_in_fork(xmalloc_test);
}

Test(xalloc, xcalloc)
{
    exec_in_fork(xcalloc_test);
}

Test(xalloc, xrealloc)
{
    exec_in_fork(xrealloc_test);
}

Test(xalloc, xfree)
{
    exec_in_fork(xfree_test);
}

Test(xalloc, xfree_null)
{
    exec_in_fork(xfree_null_test);
}

Test(xalloc, xfree_not_found, .exit_code = 1)
{
    exec_in_fork(xfree_not_found_test);
}

Test(xalloc, xfree_double_free, .exit_code = 1)
{
    exec_in_fork(xfree_double_free_test);
}

Test(xalloc, xfree_all)
{
    exec_in_fork(xfree_all_test);
}

Test(xalloc, xfree_all_null)
{
    exec_in_fork(xfree_all_null_test);
}

Test(xalloc, xfree_all_double)
{
    exec_in_fork(xfree_all_double_test);
}

Test(xalloc, xmalloc_multithreading_stress)
{
    exec_in_fork(xmalloc_multithreading_stress_test);
}
