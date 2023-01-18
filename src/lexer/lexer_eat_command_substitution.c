#include <assert.h>
#include <string.h>

#include "lexer.h"

void lexer_eat_command_substitution(lexer *lex)
{
    assert(strcmp(GET_CURRENT_CHAR_ADDR(lex), "$(") == 0);

    lex->str_token_end += 2;
    unsigned int parenthesis_count = 1;
    for (; GET_CURRENT_CHAR(lex) != '\0' && parenthesis_count > 0;
         GET_CURRENT_CHAR(lex)++)
        if (GET_CURRENT_CHAR(lex) == '(')
            parenthesis_count++;
        else if (GET_CURRENT_CHAR(lex) == ')')
            parenthesis_count--;
}
