#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "xalloc.h"

extern struct xalloc xalloc_dlist;

Test(xalloc, xalloc_init)
{
    xalloc_init();
    cr_assert_eq(xalloc_dlist.initialized, true);
    xalloc_deinit();
}

Test(xalloc, not_xalloc_init)
{
    cr_assert_eq(xalloc_dlist.initialized, false);
}

Test(xalloc, xmalloc)
{
    xalloc_init();

    void *ptr = xmalloc(42, sizeof(int));
    cr_assert_not_null(ptr);
    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.head->prev);
    cr_assert_eq(xalloc_dlist.head->ptr, ptr);

    xfree(ptr);
    cr_assert_null(xalloc_dlist.head);

    xalloc_deinit();
}

Test(xalloc, xcalloc)
{
    xalloc_init();

    void *ptr = xcalloc(42, sizeof(int));
    cr_assert_not_null(ptr);
    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_eq(xalloc_dlist.head->ptr, ptr);

    // Test if the memory is zeroed
    cr_expect_arr_eq(ptr, (int[42]){ 0 }, 42);

    xfree(ptr);
    cr_assert_null(xalloc_dlist.head);

    xalloc_deinit();
}

Test(xalloc, xrealloc)
{
    xalloc_init();

    void *ptr = xmalloc(42, sizeof(int));
    cr_assert_not_null(ptr);
    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.head->prev);
    cr_assert_eq(xalloc_dlist.head->ptr, ptr);

    ptr = xrealloc(ptr, 84, sizeof(int));
    cr_assert_not_null(ptr);
    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.head->prev);
    cr_assert_eq(xalloc_dlist.head->ptr, ptr);

    xfree(ptr);
    cr_assert_null(xalloc_dlist.head);

    xalloc_deinit();
}

Test(xalloc, xfree)
{
    xalloc_init();

    void *ptr = xmalloc(42, sizeof(int));
    cr_assert_not_null(ptr);
    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.head->prev);
    cr_assert_eq(xalloc_dlist.head->ptr, ptr);

    xfree(ptr);
    cr_assert_null(xalloc_dlist.head);

    xalloc_deinit();
}

Test(xalloc, xfree_null)
{
    xalloc_init();

    xfree(NULL);
    cr_assert_null(xalloc_dlist.head);

    xalloc_deinit();
}

Test(xalloc, xfree_not_found, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr)
{
    xalloc_init();

    xfree((void *)0x42);
    // Must not reach this line
    cr_assert_fail();
}

Test(xalloc, xfree_double_free, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr)
{
    xalloc_init();

    void *ptr = xmalloc(42, sizeof(int));
    cr_assert_not_null(ptr);
    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.head->prev);
    cr_assert_eq(xalloc_dlist.head->ptr, ptr);

    xfree(ptr);
    cr_assert_null(xalloc_dlist.head);

    xfree(ptr);
    // Must not reach this line
    cr_assert_fail();
}

Test(xalloc, xfree_all)
{
    xalloc_init();

    for (int i = 0; i < 42; i++)
    {
        xmalloc(42, sizeof(int));
    }

    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.head->prev);

    xfree_all();
    cr_assert_null(xalloc_dlist.head);

    xalloc_deinit();
}

Test(xalloc, xfree_all_null)
{
    xalloc_init();

    xfree_all();
    cr_assert_null(xalloc_dlist.head);

    xalloc_deinit();
}

Test(xalloc, xfree_all_double)
{
    xalloc_init();

    for (int i = 0; i < 42; i++)
    {
        xmalloc(42, sizeof(int));
    }

    cr_assert_not_null(xalloc_dlist.head);
    cr_assert_null(xalloc_dlist.head->prev);

    xfree_all();
    cr_assert_null(xalloc_dlist.head);

    xfree_all();
}

static void *_xmalloc_multithreading_stress_thread(void *arg)
{
    const int nb_allocs = *((int *)arg);

    for (int i = 0; i < nb_allocs; i++)
    {
        void *ptr = xmalloc(42, sizeof(int));
        cr_assert_not_null(ptr);
        cr_assert_not_null(xalloc_dlist.head);
        cr_assert_null(xalloc_dlist.head->prev);

        xfree(ptr);
        cr_assert_null(xalloc_dlist.head);
    }

    return NULL;
}

Test(xalloc, xmalloc_multithreading_stress)
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

    xalloc_deinit();
}
