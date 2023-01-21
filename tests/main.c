// Used for stuff testing

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/lexer.h"
#include "../include/xalloc.h"

int main(void)
{
    xalloc_init();

    const char *cmd = "# comment";

    token *returned_tokens = get_tokens(cmd, strlen(cmd));

    for (token *tok = returned_tokens; tok; tok = tok->next)
    {
        printf("Token type: %d, value: %s", tok->type, tok->value);
        printf("\n");
    }

    (void)returned_tokens;
    xalloc_deinit();

    return 0;
}
