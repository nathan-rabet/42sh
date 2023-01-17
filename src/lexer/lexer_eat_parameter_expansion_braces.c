#include <assert.h>
#include <string.h>

#include "lexer.h"

void lexer_eat_parameter_expansion_braces(lexer *lex)
{
    assert(strcmp(lex->str_token_end, "${") == 0);

    lex->str_token_end += 2;
    unsigned int brackets_count = 1;
    for (; *lex->str_token_end != '\0' && brackets_count; lex->str_token_end++)
        if (*lex->str_token_end == '{')
            brackets_count++;
        else if (*lex->str_token_end == '}')
            brackets_count--;
}
