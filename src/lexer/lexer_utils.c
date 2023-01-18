#include <ctype.h>

#include "lexer.h"
bool is_name(char *str, size_t len)
{
    if (len == 0)
        return false;

    if (!isalpha(str[0]) && str[0] != '_')
        return false;

    for (size_t i = 1; i < len; i++)
        if (!isalnum(str[i]) && str[i] != '_')
            return false;

    return true;
}
