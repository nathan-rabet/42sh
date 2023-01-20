#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"

static const char *tokens_mapping[] = {
    NULL, // UNDEFINED
    NULL, // WORD
    // NULL, // NAME
    NULL, // IO_NUMBER
    "\n", // NEWLINE

    "&&", // AND_IF
    "||", // OR_IF
    ";;", // DSEMI

    "<<", // DLESS
    ">>", // DGREAT
    "<&", // LESSAND
    ">&", // GREATAND
    "<>", // LESSGREAT
    "<<-", // DLESSDASH

    ">|", // CLOBBER

    "if", // IF
    "then", // THEN
    "else", // ELSE
    "elif", // ELIF
    "fi", // FI

    "case", // CASE
    "esac", // ESAC

    "while", // WHILE
    "until", // UNTIL

    "do", // DO
    "done", // DONE

    "for", // FOR

    "{", // LBRACE
    "}", // RBRACE

    "!", // BANG

    "in", // IN
    "<", // LESS
    ">", // GREAT

    ";", // SEMI
    "|", // PIPE
    "&", // AMPERSAND
    NULL, // ARITHMETIC_EXPANSION
    NULL // PARAMETER_EXPANSION
};

static inline bool is_prefix(const char *pre, const char *str, size_t pre_size)
{
    return strncmp(pre, str, pre_size) == 0;
}

token_t lexer_is_token(lexer *lex, size_t len)
{
    assert(len > 0);

    // IO_NUMBER
    if (lex->str_token_start[len - 1] == '<'
        || lex->str_token_start[len - 1] == '>')
    {
        if (len == 1)
            return (lex->str_token_start[0] == '<') ? LESS : GREAT;

        size_t i = 0;
        for (; i < len - 1; i++)
            if (!isdigit(lex->str_token_start[i]))
                break;
        if (i == len - 1)
            return IO_NUMBER;
    }

    // NEWLINE
    if (lex->str_token_start[0] == '\n')
    {
        if (len == 1)
            return NEWLINE;
        else
            return TOKEN_UNDEFINED;
    }

    // Operators and reserved words
    for (size_t i = FIRST_HARD_CODED; i <= LAST_HARD_CODED; i++)
        if (strlen(tokens_mapping[i]) == len
            && strncmp(lex->str_token_start, tokens_mapping[i], len) == 0)
            return i;

    if (is_separator(lex->str_token_start[len - 1]))
        return TOKEN_UNDEFINED;
    // else if (is_name(lex->str_token_start, len))
    //     return NAME;
    else
        return WORD;
}

token_t lexer_is_token_prefix_operator(lexer *lex, size_t len)
{
    for (size_t i = FIRST_HARD_CODED; i <= LAST_HARD_CODED; i++)
        if (is_prefix(lex->str_token_start, tokens_mapping[i], len))
            return i;

    return TOKEN_UNDEFINED;
}

token_t lexer_is_token_prefix(lexer *lex, size_t len)
{
    assert(len > 0);

    // IO_NUMBER
    size_t i = 0;
    for (; i < len - 1; i++)
        if (!isdigit(lex->str_token_start[i]))
            break;

    if (i == len - 1)
    {
        if (len == 1)
            return (lex->str_token_start[0] == '<') ? LESS : GREAT;
        else if (lex->str_token_start[len - 1] == '<'
                 || lex->str_token_start[len - 1] == '>')
            return IO_NUMBER;
    }

    // NEWLINE
    if (lex->str_token_start[0] == '\n')
    {
        if (len == 1)
            return NEWLINE;
        else
            return TOKEN_UNDEFINED;
    }

    token_t operator= lexer_is_token_prefix_operator(lex, len);
    if (operator!= TOKEN_UNDEFINED)
        return operator;

    if (is_separator(lex->str_token_start[len - 1]))
        return TOKEN_UNDEFINED;
    // else if (is_name(lex->str_token_start, len))
    //     return NAME;
    else
        return WORD;
}
