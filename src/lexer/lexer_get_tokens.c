#include <string.h>

#include "lexer.h"
#include "xalloc.h"

static inline bool is_blank(char c)
{
    return c == ' ' || c == '\t';
}

static void _get_tokens(lexer *lex, size_t input_len)
{
    assert(input_len > 0);

    char *end_addr = lex->str_token_start + input_len;
    for (; lex->str_token_end < end_addr; lex->str_token_end++)
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
                token_add(lex, prefix_token, QUOTE_NONE,
                          lex->str_token_end - lex->str_token_start);
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

            lexer_eat_quotes(lex);
            token_add(lex, WORD, quote_type,
                      lex->str_token_end - lex->str_token_start);
            break;

        case '\\':
            lexer_eat_backslash(lex);
            break;
        default:
            break;
        }

        // Rule 2.3.5: If the current character is an unquoted '$' or '`', the
        // shell shall identify the start of any candidates for parameter
        // expansion (Parameter Expansion), command substitution (Command
        // Substitution), or arithmetic expansion (Arithmetic Expansion) from
        // their introductory unquoted character sequences: '$' or "${", "$(" or
        // '`', and "$((", respectively. The shell shall read sufficient input
        // to determine the end of the unit to be expanded (as explained in the
        // cited sections). While processing the characters, if instances of
        // expansions or quoting are found nested within the substitution, the
        // shell shall recursively process them in the manner specified for the
        // construct that is found. The characters found from the beginning of
        // the substitution to its end, allowing for any recursion necessary to
        // recognize embedded constructs, shall be included unmodified in the
        // result token, including any embedded or enclosing substitution
        // operators or quotes. The token shall not be delimited by the end of
        // the substitution.
        if (*(lex->str_token_end) == '$' || *(lex->str_token_end) == '`')
        {
            // Command substitution
            if (strcmp(lex->str_token_end, '$(') == 0)
                lexer_eat_command_substitution(lex);
            else if (*(lex->str_token_end) == '`')
                lexer_eat_command_backquote(lex);
            // Arithmetic expansion
            else if (strcmp(lex->str_token_end, "$((") == 0)
                lexer_eat_arithmetics_expansion(lex);
            // Parameter expansion
            else if (strcmp(lex->str_token_end, "${") == 0)
                lexer_eat_parameter_expansion_braces(lex);
            else if (*(lex->str_token_end) == '$')
                lexer_eat_parameter_expansion(lex);

            token_add(lex, WORD, QUOTE_NONE,
                      lex->str_token_end - lex->str_token_start);
            lex->str_token_start = lex->str_token_end;
        }
    }

    // Rule 2.3.1: If the end of input is recognized, the current token (if any)
    // shall be delimited.
    if (lex->str_token_start != lex->str_token_end)
        token_add(lex, WORD, QUOTE_NONE,
                  lex->str_token_end - lex->str_token_start);
}

token *get_tokens(const char *input, size_t len)
{
    lexer *lex = xcalloc(1, sizeof(lexer));
    lex->str = input;
    lex->str_token_start = input;
    lex->str_token_end = input;
    lex->tokens = NULL;
    _get_tokens(lex, len);

    token_t *tokens = lex->tokens;
    xfree(lex);
    return tokens;
}