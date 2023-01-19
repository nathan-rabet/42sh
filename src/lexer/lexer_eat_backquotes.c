#include <assert.h>

#include "lexer.h"

void lexer_eat_command_backquote(lexer *lex)
{
    assert(GET_CURRENT_CHAR(lex) == '`');

    char quote_c = GET_CURRENT_CHAR(lex);
    lex->str_token_end++;

    // 2.2.2 Single-quotes: Preserve literal value of each character
    // if within single quotes.
    for (; !IS_END_OF_INPUT(lex) && GET_CURRENT_CHAR(lex) != quote_c;
         lex->str_token_end++)
        if (GET_CURRENT_CHAR(lex) == '\\')
            lexer_eat_backslash(lex);
}
