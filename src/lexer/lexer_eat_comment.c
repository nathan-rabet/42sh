#include <assert.h>

#include "lexer.h"

void lexer_eat_comment(lexer *lex)
{
    assert(GET_CURRENT_CHAR(lex) == '#');

    while (GET_CURRENT_CHAR(lex) != '\0' && GET_CURRENT_CHAR(lex) != '\n')
        lex->str_token_end++;

    if (GET_CURRENT_CHAR(lex) == '\n')
        lex->str_token_end++; // Remove the newline.
}
