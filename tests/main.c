// Used for stuff testing

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/lexer.h"
#include "../include/xalloc.h"
#include "builtins.h"

int main(void)
{
    xalloc_init();

    char *cmd[] = { "alias", "foo=bar", NULL };
    char *cmd1[] = { "alias", "foo1=bar1", NULL };
    char *cmd2[] = { "alias", NULL };

    builtin_alias(cmd + 1);
    builtin_alias(cmd1 + 1);
    builtin_alias(cmd2 + 1);

    xalloc_deinit();

    return 0;
}
