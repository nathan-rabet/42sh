#include <assert.h>

#include "lexer.h"

char *lexer_eat_backslash(token_t tokens, char *str)
{
    assert(str[0] == '\\');

    if (str[1] == '\n')
        str += 2; // Remove the backslash and the newline.

    else
        str++; // Remove the backslash.

    return str;
}
