#include <assert.h>

#include "../../include/lexer.h"

void lexer_eat_comment(lexer *lex)
{
    assert(GET_CURRENT_CHAR(lex) == '#');

    while (!IS_END_OF_INPUT(lex) && GET_CURRENT_CHAR(lex) != '\n')
        lex->str_token_end++;

    lex->str_token_start = lex->str_token_end;
}
