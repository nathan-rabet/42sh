#include <assert.h>
#include <err.h>
#include <string.h>

#include "lexer.h"

static void remove_input_character_forward(lexer *lex, size_t len)
{
    memmove(GET_CURRENT_CHAR_ADDR(lex), GET_CURRENT_CHAR_ADDR(lex) + len,
            lex->input_len - (GET_CURRENT_CHAR_ADDR(lex) - lex->input) - len);

    // Adding \0 to the end of the string.
    lex->input_len -= len;
    lex->input[lex->input_len] = '\0';
}

void lexer_eat_newline_joining(lexer *lex)
{
    assert(GET_CURRENT_CHAR(lex) == '\\' && GET_NEXT_CHAR(lex) == '\n');

    remove_input_character_forward(lex, 2);
}

void lexer_eat_backslash(lexer *lex)
{
    assert(GET_CURRENT_CHAR(lex) == '\\');

    if (!HAS_NEXT_CHAR(lex))
        errx(1, "lexing: Backslash at the end of the input.");
    else
        remove_input_character_forward(lex, 1);
}
