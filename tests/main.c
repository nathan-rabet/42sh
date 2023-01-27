// Used for stuff testing

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/lexer.h"
#include "../include/xalloc.h"
#include "alias.h"

int main(void)
{
    xalloc_init();
    add_alias("foo", "bar baz");

    const char *cmd = "foo bar baz";

    token *returned_tokens = get_tokens(cmd, strlen(cmd));

    for (token *tok = returned_tokens; tok; tok = tok->next)
    {
        printf("Token type: %d, value: %s", tok->type, tok->value);
        printf("\n");
    }

    xalloc_deinit();

    return 0;
}
