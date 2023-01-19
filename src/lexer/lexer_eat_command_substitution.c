#include <assert.h>
#include <string.h>

#include "lexer.h"

void lexer_eat_command_substitution(lexer *lex)
{
    assert(strcmp(GET_CURRENT_CHAR_ADDR(lex), "$(") == 0);

    lex->str_token_end += 2;
    unsigned int parenthesis_count = 1;
    for (; !IS_END_OF_INPUT(lex) && parenthesis_count > 0;
         GET_CURRENT_CHAR(lex)++)
        if (CAN_EAT_NEWLINE_JOINING(lex))
            lexer_eat_newline_joining(lex);
        else if (GET_CURRENT_CHAR(lex) == '(')
            parenthesis_count++;
        else if (GET_CURRENT_CHAR(lex) == ')')
            parenthesis_count--;
}
