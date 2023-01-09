#include <stddef.h>

#include "lexer.h"
#include "xalloc.h"

lexer *lexer_init(const char *str)
{
    lexer *lex = xmalloc(1, sizeof(lexer));
    lex->str = str;
    lex->str_i = 0;
    lex->current_token = NULL;

    return lex;
}
