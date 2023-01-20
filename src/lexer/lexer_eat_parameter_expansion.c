#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "../../include/lexer.h"

void lexer_eat_parameter_expansion(lexer *lex)
{
    assert(GET_CURRENT_CHAR(lex) == '$');

    lex->str_token_end += 1;
    while (!IS_END_OF_INPUT(lex)
           && is_name(GET_CURRENT_CHAR_ADDR(lex), GET_LEN_CURRENT_CHAR(lex)))
        lex->str_token_end++;
}
