#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"

static const char *tokens_mapping[] = {
    NULL, // UNDEFINED
    NULL, // WORD
    NULL, // NAME
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

    "while" // WHILE
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
    NULL, // ARITHMETIC_EXPANSION
    NULL // PARAMETER_EXPANSION
};

static inline bool is_prefix(const char *pre, const char *str, size_t pre_size)
{
    return strncmp(pre, str, pre_size) == 0;
}

token_t lexer_is_token_prefix(lexer *lex, size_t pre_size)
{
    assert(pre_size > 0);

    // IO_NUMBER
    if (lex->str_token_start[0] == '<' || lex->str_token_start[0] == '>')
    {
        if (pre_size == 1)
            return IO_NUMBER;
        else
        {
            for (size_t i = 1; i < pre_size; i++)
                if (!isdigit(lex->str_token_start[i]))
                    goto ret;
            return IO_NUMBER;
        }
    }

    // NEWLINE
    if (lex->str_token_start[0] == '\n')
        return NEWLINE;

    for (size_t i = FIRST_HARD_CODED; i < LAST_HARD_CODED; i++)
        if (is_prefix(lex->str_token_start, tokens_mapping[i], pre_size))
            return i;

ret:
    return (is_name(lex->str_token_start, pre_size) ? NAME : WORD);
}
