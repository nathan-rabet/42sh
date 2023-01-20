#include <ctype.h>

#include "lexer.h"

bool is_name(const char *str, size_t len)
{
    if (len == 0)
        return false;

    if (str[0] == '_' || isalpha(str[0]))
    {
        for (size_t i = 1; i < len; i++)
            if (str[i] != '_' && !isalnum(str[i]))
                return false;
        return true;
    }

    return false;
}

bool is_separator(char c)
{
    // metacharacter: A character that, when unquoted, separates words. One of
    // the following:
    // || & && ; ;; ;& ;;& ( ) | |& <newline>

    // Adding < and > to the list of separators
    return IS_BLANK(c) || c == ';' || c == '&' || c == '|' || c == '('
        || c == ')' || c == '\n' || c == '<' || c == '>';
}
