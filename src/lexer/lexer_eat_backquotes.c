#include <assert.h>

#include "lexer.h"

void lexer_eat_command_backquote(lexer *lex)
{
    assert(lex->str_token_end[0] == '`');

    char quote_c = lex->str_token_end[0];

    // 2.2.2 Single-quotes: Preserve literal value of each character
    // if within single quotes.
    for (; lex->str_token_end[0] != '\0' && lex->str_token_end[0] != quote_c;
         lex->str_token_end++)
        if (lex->str_token_end[0] == '\\')
            lexer_eat_backslash(lex);
}
