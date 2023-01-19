// Used for stuff testing

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "lexer.h"
#include "xalloc.h"

int main(void)
{
    xalloc_init();

    const char *cmd = "echo test eeee";

    token *returned_tokens = get_tokens(cmd, strlen(cmd));

    assert(returned_tokens->type == WORD);
    xalloc_deinit();

    return 0;
}
