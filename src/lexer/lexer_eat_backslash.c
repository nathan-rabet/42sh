#include <assert.h>
#include <string.h>

#include "lexer.h"

void lexer_eat_newline_joining(lexer *lex)
{
    assert(GET_CURRENT_CHAR(lex) == '\\' && GET_NEXT_CHAR(lex) == '\n');

    // Join the next line to the current one.
    memmove(GET_CURRENT_CHAR_ADDR(lex), GET_CURRENT_CHAR_ADDR(lex) + 2,
            lex->input_len - GET_LEN_CURRENT_CHAR(lex) - 2 + 1);
    lex->input_len -= 2;
}

void lexer_eat_backslash(lexer *lex)
{
    assert(GET_CURRENT_CHAR(lex) == '\\');

    if (!HAS_NEXT_CHAR(lex))
        lex->str_token_end++;

    else if (GET_NEXT_CHAR(lex) == '\n')
        lexer_eat_newline_joining(lex);
    else
        lex->str_token_end += 2; // Bypass the backslash and the next character.
}
