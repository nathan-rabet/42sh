#include <assert.h>
#include <string.h>

#include "../../include/lexer.h"
#include "../../include/xalloc.h"

static void _get_tokens(lexer *lex)
{
    assert(lex->input_len > 0);

    for (; !IS_END_OF_INPUT(lex); lex->str_token_end++)
    {
        // Rule 2.3.2 : If the previous character was used as part of an
        // operator and the current character is not quoted and can be used with
        // the previous characters to form an operator, it shall be used as part
        // of that (operator) token.
        if (HAS_PREVIOUS_CHAR(lex))
        {
            token_t prefix_previous_token =
                lexer_is_token_prefix_operator(lex, GET_LEN_PREVIOUS_CHAR(lex));

            if (IS_OPERATOR(prefix_previous_token))
            {
                token_t prefix_current_token = lexer_is_token_prefix_operator(
                    lex, GET_LEN_CURRENT_CHAR(lex));
                if (IS_OPERATOR(prefix_current_token))
                    continue;
            }
        }

        // Rule 2.3.3: If the previous character was used as part of an operator
        // and the current character cannot be used with the previous characters
        // to form an operator, the operator containing the previous character
        // shall be delimited.
        if (HAS_PREVIOUS_CHAR(lex))
        {
            token_t prefix_previous_token =
                lexer_is_token_prefix_operator(lex, GET_LEN_PREVIOUS_CHAR(lex));

            if (IS_OPERATOR(prefix_previous_token))
            {
                token_t current_token =
                    lexer_is_token(lex, GET_LEN_CURRENT_CHAR(lex));
                if (!IS_OPERATOR(current_token))
                {
                    token_t previous_token =
                        lexer_is_token(lex, GET_LEN_PREVIOUS_CHAR(lex));
                    token_add(lex, previous_token, GET_LEN_PREVIOUS_CHAR(lex));
                    // continue;
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
        switch (GET_CURRENT_CHAR(lex))
        {
        case '"':
        case '\'':
            lexer_eat_quotes(lex);
            token_add(lex, WORD, GET_LEN_CURRENT_CHAR(lex));
            continue;
        case '\\':
            if (CAN_EAT_NEWLINE_JOINING(lex))
            {
                lexer_eat_newline_joining(lex);
                lex->str_token_end--; // Next loop will increment it.
                continue;
            }
            else
            {
                lexer_eat_backslash(lex);
                break;
            }
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
        if (GET_CURRENT_CHAR(lex) == '$' || GET_CURRENT_CHAR(lex) == '`')
        {
            // Command substitution
            if (strncmp(GET_CURRENT_CHAR_ADDR(lex), "$(", 2) == 0)
                lexer_eat_command_substitution(lex);
            else if (GET_CURRENT_CHAR(lex) == '`')
                lexer_eat_command_backquote(lex);
            // Arithmetic expansion
            else if (strncmp(GET_CURRENT_CHAR_ADDR(lex), "$((", 3) == 0)
                lexer_eat_arithmetics_expansion(lex);
            // Parameter expansion
            else if (strncmp(GET_CURRENT_CHAR_ADDR(lex), "${", 2) == 0)
                lexer_eat_parameter_expansion_braces(lex);
            else if (GET_CURRENT_CHAR(lex) == '$')
                lexer_eat_parameter_expansion(lex);

            token_add(lex, WORD, GET_LEN_CURRENT_CHAR(lex));
            continue;
        }

        // Rule 2.3.6: If the current character is not quoted and can be used as
        // the first character of a new operator, the current token (if any)
        // shall be delimited. The current character shall be used as the
        // beginning of the next (operator) token.
        char *tmp_str_token_start = lex->str_token_start;
        lex->str_token_start = GET_CURRENT_CHAR_ADDR(lex);
        token_t token = lexer_is_token_prefix(lex, 1);
        lex->str_token_start = tmp_str_token_start;
        if (HAS_PREVIOUS_CHAR(lex) && IS_OPERATOR(token))
        {
            token_add(lex, WORD, GET_LEN_PREVIOUS_CHAR(lex));
            continue;
        }

        // Rule 2.3.7: If the current character is an unquoted <blank>, any
        // token containing the previous character is delimited and the current
        // character shall be discarded.
        if (IS_BLANK(GET_CURRENT_CHAR(lex)))
        {
            if (HAS_PREVIOUS_CHAR(lex))
                token_add(lex, lexer_is_token(lex, GET_LEN_PREVIOUS_CHAR(lex)),
                          GET_LEN_PREVIOUS_CHAR(lex));

            // Discard current character
            lex->str_token_start++;
            continue;
        }

        // Rule 2.3.8: If the previous character was part of a word, the current
        // character shall be appended to that word.
        if (HAS_PREVIOUS_CHAR(lex)
            && (lexer_is_token_prefix(lex, GET_LEN_PREVIOUS_CHAR(lex))
                != TOKEN_UNDEFINED)
            && !is_separator(GET_PREVIOUS_CHAR(lex))
            && !is_separator(GET_CURRENT_CHAR(lex)))
            continue;

        // Rule 2.3.9: If the current character is a '#', it and all subsequent
        // characters up to, but excluding, the next <newline> shall be
        // discarded as a comment. The <newline> that ends the line is not
        // considered part of the comment.
        if (GET_CURRENT_CHAR(lex) == '#')
        {
            lexer_eat_comment(lex);
            continue;
        }

        // Rule 2.3.10: The current character is used as the start of a new
        // word.
        if (HAS_PREVIOUS_CHAR(lex))
        {
            token_add(lex, lexer_is_token(lex, GET_LEN_PREVIOUS_CHAR(lex)),
                      GET_LEN_PREVIOUS_CHAR(lex));
            lex->str_token_end--;
        }
    }

    // Rule 2.3.1: If the end of input is recognized, the current token (if
    // any) shall be delimited.
    if (lex->str_token_start != GET_CURRENT_CHAR_ADDR(lex))
        token_add(lex, lexer_is_token(lex, GET_LEN_PREVIOUS_CHAR(lex)),
                  GET_LEN_PREVIOUS_CHAR(lex));
}

token *get_tokens(const char *input, size_t len)
{
    if (len == 0)
        return NULL;

    lexer lex = { 0 };
    char *input_cpy = xcalloc(len + 1, sizeof(char));
    lex.input = memcpy(input_cpy, input, len);
    lex.input_len = len;
    lex.str_token_start = input_cpy;
    lex.str_token_end = input_cpy + 1;
    lex.tokens = NULL;
    _get_tokens(&lex);

    token *tokens = lex.tokens;
    xfree(input_cpy);
    return tokens;
}
