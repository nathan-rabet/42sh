#include <string.h>

#include "lexer.h"
#include "xalloc.h"

#define BUFFER_SIZE 1024

char *IFS = " \t\n";

static bool is_prefix(const char *pre, const char *str, size_t pre_size)
{
    return strncmp(pre, str, pre_size) == 0;
}

static void find_next_char(const char *str, char fond_char)
{
    size_t i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == fond_char)
            return i;
        i++;
    }
    return -1;
}

token *get_tokens(const char *input, size_t *tokens_len)
{
    lexer lexer = { 0 };
    lexer.str = input;

    for (; lexer.str[lexer.str_token_end] != '\0'; lexer.str_token_end++)
    {
        // Rule 2.3.2 : If the previous character was used as part of an
        // operator and the current character is not quoted and can be used with
        // the previous characters to form an operator, it shall be used as part
        // of that (operator) token.

        // Check if previous characters form an operator prefix.
        int i = FIRST_OPERATOR;
        for (; i <= LAST_OPERATOR; i++)
        {
            if (is_prefix(lexer.str + lexer.str_token_start, tokens_mapping[i],
                          lexer.str_token_end - lexer.str_token_start - 1))
                break;
        }

        if (i <= LAST_OPERATOR)
        {
            /**
             * Check if current character with previous characters can form an
             * operator prefix (must be true for Rule 2.3.2 and false for
             * Rule 2.3.3).
             */
            for (i = FIRST_OPERATOR; i <= LAST_OPERATOR; i++)
            {
                if (is_prefix(lexer.str + lexer.str_token_start,
                              tokens_mapping[i],
                              lexer.str_token_end - lexer.str_token_start))
                    break;
            }

            if (i <= LAST_OPERATOR && !IS_CURRENT_IN_QUOTES(lexer))
            {
                // A prefix can be formed using current character (which is not
                // in quotes).
                continue;
            }
            else if (i > LAST_OPERATOR)
            {
                // Otherwise create token if an operator can be formed with
                // current character (Rule 2.3.3).
                token_t token_type = get_string_operator(
                    lexer.str + lexer.str_token_start,
                    lexer.str_token_end - lexer.str_token_start);

                if (token_type != TOKEN_UNDEFINED)
                {
                    lexer.tokens = xrealloc(lexer.tokens, ++lexer.nb_tokens,
                                            sizeof(token));
                    lexer.tokens[lexer.nb_tokens - 1].type = token_type;
                    lexer.tokens[lexer.nb_tokens - 1].value = NULL;
                    lexer.str_token_start = lexer.str_token_end + 1;
                }
            }
        }
    }

    // Rule 2.3.1: If the end of input is recognized, the current token (if any)
    // shall be delimited.
    if (lexer.str_token_start != lexer.str_token_end)
    {
        lexer.tokens = xrealloc(lexer.tokens, ++lexer.nb_tokens, sizeof(token));
        lexer.tokens[lexer.nb_tokens - 1].type = WORD;
        lexer.tokens[lexer.nb_tokens - 1].value =
            strcpy(xmalloc(lexer.str_token_end - lexer.str_token_start + 1,
                           sizeof(char)),
                   lexer.str + lexer.str_token_start);
    }
}
