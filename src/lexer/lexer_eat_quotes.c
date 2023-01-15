#include <assert.h>

#include "lexer.h"

char *lexer_eat_quotes(token_t tokens, char *str)
{
    assert(str[0] == '\'' || str[0] == '"');

    char quote_c = str[0];

    // 2.2.2 Single-quotes: Preserve literal value of each character
    // if within single quotes.
    for (; str[0] != '\0' && str[0] != quote_c; str++)
        if (str[0] == '\\')
            str = lexer_eat_backslash(tokens, str);

    if (str[0] == '\0')
        err(2,
            "Missing terminating quote for quote delimited "
            "text");

    return str + 1;
}
