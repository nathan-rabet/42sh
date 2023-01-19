#include <assert.h>
#include <string.h>

#include "lexer.h"

void lexer_eat_parameter_expansion_braces(lexer *lex)
{
    assert(strncmp(GET_CURRENT_CHAR_ADDR(lex), "${", 2) == 0);

    lex->str_token_end += 2;
    unsigned int brackets_count = 1;
    for (; !IS_END_OF_INPUT(lex) && brackets_count; lex->str_token_end++)
        if (GET_CURRENT_CHAR(lex) == '{')
            brackets_count++;
        else if (GET_CURRENT_CHAR(lex) == '}')
            brackets_count--;
}
