#include <assert.h>
#include <string.h>

#include "lexer.h"

void lexer_eat_command_substitution(lexer *lex)
{
    assert(strncmp(GET_CURRENT_CHAR_ADDR(lex), "$(", 2) == 0);

    lex->str_token_end += 2;
    unsigned int parenthesis_count = 1;
    for (; !IS_END_OF_INPUT(lex) && parenthesis_count > 0; lex->str_token_end++)
        if (CAN_EAT_NEWLINE_JOINING(lex))
            lexer_eat_newline_joining(lex);
        else if (GET_CURRENT_CHAR(lex) == '(')
            parenthesis_count++;
        else if (GET_CURRENT_CHAR(lex) == ')')
            parenthesis_count--;
}
