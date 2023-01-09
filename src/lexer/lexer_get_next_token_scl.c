#include <string.h>

#include "lexer.h"
#include "xalloc.h"

#define BUFFER_SIZE 1024

char *IFS = " \t\n";

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

token *get_next_tokens(const char *input, size_t str_len, size_t *tokens_len)
{
    lexer lexer = { 0 };
    lexer.str = input;

    size_t nb_tokens = 0;
    token *tokens = NULL;

    // TODO : implement infinite length tokens
    for (; lexer.str[lexer.value_end] != '\0'; lexer.value_end++)
    {
        // Rule 2.3.2
        if (tokens && IS_OPERATOR(tokens[nb_tokens - 1].type)
            && !IS_IN_QUOTES(tokens[nb_tokens]))
        {
            // Create token if current characters (previous characters plus new
            // one) form an operator.
            for (int i = FIRST_OPERATOR; i <= LAST_OPERATOR + 1; i++)
            {
                char *potential_token_start = lexer.str + lexer.value_start;
                if (strncmp(tokens_mapping[i], potential_token_start,
                            strlen(tokens_mapping[i]))
                    == 0)
                {
                    tokens = xrealloc(tokens, ++nb_tokens, sizeof(token));
                    tokens[nb_tokens - 1].type = token_types[i];
                    tokens[nb_tokens - 1].value = NULL;
                }
            }

            /*
            lexer.last_token.type = UNDEFINED;
            lexer.last_token.value = NULL;
            */
        }
    }

    // Rule 2.3.1: If the end of input is recognized, the current token (if any)
    // shall be delimited.
    if (lexer.last_token.type == UNDEFINED)
    {
        tokens = xrealloc(tokens, ++nb_tokens, sizeof(token));
        tokens[nb_tokens - 1].type = WORD;
        // TODO(robertnant): use index instead of "last_token"
        tokens[nb_tokens - 1].value = lexer.last_token.value;
    }
}
