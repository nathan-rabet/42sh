// Used for stuff testing

#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "xalloc.h"

extern struct xalloc xalloc_dlist;

void func(void)
{
    xalloc_init();

    void *ptr = xmalloc(42, sizeof(int));

    xfree(ptr);

    xalloc_deinit();

    printf("Done\n");
}

int main(void)
{
    pid_t pid = fork();

    if (pid == 0) // Child process
        func();
    else if (pid > 0) // Parent process
    {
        if (wait(NULL) == -1)
            errx(EXIT_FAILURE, "Failed to wait for child process.");
    }
    else // Error
        errx(EXIT_FAILURE, "Failed to fork process.");

    return 0;
}
