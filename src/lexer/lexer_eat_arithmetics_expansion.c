#include <assert.h>
#include <string.h>

#include "../../include/lexer.h"

void lexer_eat_arithmetics_expansion(lexer *lex)
{
    assert(strncmp(lex->str_token_end, "$((", 3) == 0);

    lex->str_token_end += 3;
    unsigned int parenthesis_count = 2;
    for (; !IS_END_OF_INPUT(lex) && parenthesis_count > 0; lex->str_token_end++)
        if (GET_CURRENT_CHAR(lex) == '(')
            parenthesis_count++;
        else if (GET_CURRENT_CHAR(lex) == ')')
            parenthesis_count--;
}
