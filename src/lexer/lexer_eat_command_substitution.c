#include <assert.h>
#include <string.h>

#include "lexer.h"

void lexer_eat_command_substitution(lexer *lex)
{
    assert(strcmp(lex->str_token_end, "$(") == 0);

    lex->str_token_end += 2;
    unsigned int parenthesis_count = 1;
    for (; *lex->str_token_end != '\0' && parenthesis_count;
         lex->str_token_end++)
        if (*lex->str_token_end == '(')
            parenthesis_count++;
        else if (*lex->str_token_end == ')')
            parenthesis_count--;
}
