#include <string.h>

#include "lexer.h"
#include "xalloc.h"

#define BUFFER_SIZE 1024

char *IFS = " \t\n";

static bool is_prefix(const char *pre, const char *str, size_t pre_size)
{
    return strncmp(pre, str, pre_size) == 0;
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

        // Rule 2.3.4: If the current character is <backslash>, single-quote, or
        // double-quote and it is not quoted, it shall affect quoting for
        // subsequent characters up to the end of the quoted text. The rules for
        // quoting are as described in Quoting . During token recognition no
        // substitutions shall be actually performed, and the result token shall
        // contain exactly the characters that appear in the input (except for
        // <newline> joining), unmodified, including any embedded or enclosing
        // quotes or substitution operators, between the <quotation-mark> and
        // the end of the quoted text. The token shall not be delimited by the
        // end of the quoted field.
        bool is_quote_or_backslash = lexer.str[lexer.str_token_end] == '\\'
            || lexer.str[lexer.str_token_end] == '\''
            || lexer.str[lexer.str_token_end] == '"';
        if (!IS_CURRENT_IN_QUOTES(lexer))
        {
            switch (lexer.str[lexer.str_token_end])
            {
            case '"':
            case '\'':
                char quote_c = lexer.str[lexer.str_token_end];

                lexer.quote = (quote_c == '"' ? QUOTE_DOUBLE : QUOTE_SINGLE);

                for (; lexer.str[lexer.str_token_end] != '\0'
                     && lexer.str[lexer.str_token_end] != quote_c;
                     lexer.str_token_end++)
                {
                    // 2.2.2 Single-Quotes
                    if (quote_c == '\'')
                    {}

                    // 2.2.3 Double-Quotes
                    if (quote_c == '"')
                    {
                        if (lexer.str[lexer.str_token_end] == '$')
                        {
                            if (lexer.str[lexer.str_token_end + 1] == '{')
                            {
                                lexer.str_token_end += 2;
                                for (; lexer.str[lexer.str_token_end] != '}'
                                     && lexer.str[lexer.str_token_end] != '\0';
                                     lexer.str_token_end++)
                                {}
                            }
                            else
                            {
                                lexer.str_token_end++;
                                for (; lexer.str[lexer.str_token_end] != ' '
                                     && lexer.str[lexer.str_token_end] != '\0';
                                     lexer.str_token_end++)
                                {}
                            }
                        }
                    }
                }
                break;

            case '\\':
                lexer.str_token_end++;
                break;
            default:
                break;
            }

            if (lexer.str[lexer.str_token_end] == '"')
            {}

            else if (lexer.str[lexer.str_token_end] == '\'')
                lexer.quote =
                    (lexer.quote == QUOTE_SINGLE ? QUOTE_NONE : QUOTE_SINGLE);

            else if (lexer.str[lexer.str_token_end] == '\\')
                lexer.str_token_end++;
            // TODO(robertnant): maybe indicate that next characters will be
            // quoted(except if current is \)
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
