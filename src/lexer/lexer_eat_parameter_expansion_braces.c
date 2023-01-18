#include <assert.h>
#include <string.h>

#include "lexer.h"

void lexer_eat_parameter_expansion_braces(lexer *lex)
{
    assert(strcmp(GET_CURRENT_CHAR_ADDR(lex), "${") == 0);

    lex->str_token_end += 2;
    unsigned int brackets_count = 1;
    for (; GET_CURRENT_CHAR(lex) != '\0' && brackets_count;
         lex->str_token_end++)
        if (GET_CURRENT_CHAR(lex) == '{')
            brackets_count++;
        else if (GET_CURRENT_CHAR(lex) == '}')
            brackets_count--;
}
