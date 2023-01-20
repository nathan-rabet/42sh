#include <assert.h>

#include "lexer.h"

void lexer_eat_quotes(lexer *lex)
{
    assert(GET_CURRENT_CHAR(lex) == '\'' || GET_CURRENT_CHAR(lex) == '"');

    char quote_c = GET_CURRENT_CHAR(lex);
    lex->str_token_end++;

    // 2.2.2 Single-quotes: Preserve literal value of each character
    // if within single quotes.
    for (; !IS_END_OF_INPUT(lex) && GET_CURRENT_CHAR(lex) != quote_c;
         lex->str_token_end++)

        // Newline joining
        if (GET_CURRENT_CHAR(lex) == '\\' && HAS_NEXT_CHAR(lex)
            && GET_NEXT_CHAR(lex) == '\n')
            lexer_eat_newline_joining(lex);
}
