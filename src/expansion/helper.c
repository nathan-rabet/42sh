#include "evalexpr.h"

// Checks if char is operator.
int is_operator(char c)
{
    char valid_chars[] = { '+', '-', '*', '/', '%', '^', '\0' };

    for (int i = 0; valid_chars[i]; i++)
    {
        if (c == valid_chars[i])
            return 1;
    }

    return 0;
}

// Check if character is valid depending on evaluation type.
int valid(char c, int rpn)
{
    char valid_chars[] = { '+', '-', '*', '/', '%', '^', ' ', '\0' };

    int cond1 = c >= '0' && c <= '9';

    // Check if c is a parenthesis and rpn.
    // (Parenthesis only valid in infix mode).
    if ((c == '(' || c == ')') && rpn)
        return 0;
    else if (c == '(' || c == ')')
        return 1;

    for (int i = 0; valid_chars[i]; i++)
    {
        if (c == valid_chars[i])
            return 1;
    }

    return cond1 ? 1 : 0;
}

// Parses number starting at given index.
// i index at end will point to char after last digit.
int parse_number(char *str, size_t *i)
{
    int res = 0;

    while (str[*i] != '\0' && (str[*i] >= '0' && str[*i] <= '9'))
    {
        int c = str[*i];
        res *= 10;
        res += c - '0';

        *i += 1;
    }

    return res;
}

// Skips spaces.
// i index at end will point to char after last digit.
void skip_spaces(char *str, size_t *i)
{
    while (str[*i] != '\0' && str[*i] == ' ')
    {
        *i += 1;
    }
}
