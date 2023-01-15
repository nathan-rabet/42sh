#include <string.h>

#include "lexer.h"
#include "xalloc.h"
static void token_add(lexer *l, token_t token_type, quote_t quote_type,
                      size_t value_size);
{
    struct token *new = xmalloc(1, sizeof(token));

    new->type = token_type;
    new->quote = quote_type;
    new->value = xcalloc(1, value_size);
    memcpy(new->value, l->str_token_start, value_size);
    new->next = NULL;

    if (l->tokens == NULL)
        l->tokens = new;
    else
        for (struct token *tmp = l->tokens; tmp != NULL; tmp = tmp->next)
            if (tmp->next == NULL)
            {
                tmp->next = new;
                break;
            }
}

/**
 * Handle's backquote when detected within double quotes.
 * lexer_input represents a pointer to the the lexer's input string
 * one byte after the backquote within the double quotes that has been detected.
 * After running this function, it is expected that the string pointed by
 * lexer_input should point to the ending backquote found.
 *
 * Return: final command that will be used for command expansion
 * (string within backquotes without the escaped characters).
 */
static char *handle_backquote_in_quoting(lexer *lexer)
{
    char *expansion_command = NULL;
    size_t expansion_command_index = 0;

    char current_char = '\0';
    while ((current_char = lexer->str_token_end) != '\0')
    {
        char previous_char = *(lexer->str_token_end - 1);

        // Case if '\' is found.
        if (previous_char == '\\')
        {
            if (current_char == '$')
            {
                // "... ... `$`"
            }
            else if (current_char == '`')
            {}
            else if (current_char == '\\')
            {}
            else
            {}
        }

        expansion_command = xcalloc(expansion_command_index + 1, sizeof(char));
        expansion_command[expansion_command_index++] = current_char;
        if (current_char == '`' && previous_char != '\'')
        {
            // Ending backquote found.
            lexer->str_token_end++;
            break;
        }

        lexer->str_token_end++;
    }

    if (current_char == '\0')
        err(2, "Lexer: Missing ending backquote");

    // Null terminate expansion command.
    expansion_command[expansion_command_index] = '\0';

    return expansion_command;
}

static void _get_tokens(lexer *lex, size_t input_len)
{
    assert(input_len > 0);
    for (; input_len--; lex->str_token_end++)
    {
        // Rule 2.3.2 : If the previous character was used as part of an
        // operator and the current character is not quoted and can be used with
        // the previous characters to form an operator, it shall be used as part
        // of that (operator) token.

        // Check if previous characters form an operator prefix.
        token_t prefix_token = lexer_is_token_prefix(
            lex, lex->str_token_end - lex->str_token_start - 1);

        // If an operator prefix can be formed
        if (prefix_token >= FIRST_OPERATOR && prefix_token <= LAST_OPERATOR)
        {
            /**
             * Check if current character with previous characters can form an
             * operator prefix (must be true for Rule 2.3.2 and false for
             * Rule 2.3.3).
             */
            prefix_token = lexer_is_token_prefix(
                lex, lex->str_token_end - lex->str_token_start);

            if (prefix_token >= FIRST_OPERATOR && prefix_token <= LAST_OPERATOR)
                continue; // a prefix can be formed using current character
                          // (which is not in quotes).
            else
            {
                token_add(lex, prefix_token, QUOTE_NONE,
                          lex->str_token_end - lex->str_token_start);
                lex->str_token_start = lex->str_token_end;
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
        switch (*(lex->str_token_end))
        {
        case '"':
        case '\'':
            quote_t quote_type =
                *(lex->str_token_end) == '"' ? QUOTE_DOUBLE : QUOTE_SINGLE;

            lex->str_token_end =
                lexer_eat_quotes(lex->tokens, lex->str_token_end);
            token_add(lex, WORD, quote_type,
                      lex->str_token_end - lex->str_token_start);

            lex->str_token_start = lex->str_token_end;
            break;

        case '\\':
            lex->str_token_end =
                lexer_eat_backslash(lex->tokens, &lex->str_token_end);
            break;
        default:
            break;
        }
    }

    // Rule 2.3.1: If the end of input is recognized, the current token (if any)
    // shall be delimited.
    if (lex->str_token_start != lex->str_token_end)
    {
        lex->tokens = token_add(
            lex->tokens, WORD,
            strcpy(xmalloc(lex->str_token_end - lex->str_token_start + 1,
                           sizeof(char)),
                   lex->str_token_start));
    }
}

token *get_tokens(const char *input)
{
    lexer *lex = xcalloc(1, sizeof(lexer));
    lex->str = input;
    lex->str_token_start = input;
    lex->str_token_end = input;
    lex->tokens = NULL;
    _get_tokens(lex, strlen(input));

    token_t *tokens = lex->tokens;
    xfree(lex);
    return tokens;
}
