#include <assert.h>
#include <string.h>

#include "lexer.h"

void lexer_eat_parameter_expansion(lexer *lex)
{
    assert(strcmp(lex->str_token_end, "$") == 0);

    lex->str_token_end += 1;
    while (GET_CURRENT_CHAR(lex) != '\0'
           && is_name(GET_CURRENT_CHAR_ADDR(lex), GET_LEN_CURRENT_CHAR(lex)))
        lex->str_token_end++;
}
