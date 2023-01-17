#include <assert.h>
#include <string.h>

#include "lexer.h"

void lexer_eat_parameter_expansion(lexer *lex)
{
    assert(strcmp(lex->str_token_end, "$") == 0);

    lex->str_token_end += 1;
    while (lex->str_token_end != '\0'
           && is_name(lex->str_token_end,
                      lex->str_token_end - lex->str_token_start))
        lex->str_token_end++;
}
