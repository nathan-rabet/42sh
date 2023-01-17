#include <assert.h>

#include "lexer.h"

void lexer_eat_backslash(lexer *lex)
{
    assert(lex->str_token_end[0] == '\\');

    if (lex->str_token_end[1] == '\n')
        lex->str_token_end += 2; // Remove the backslash and the newline.

    else
        lex->str_token_end++; // Remove the backslash.
}
