#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef int token_t;

typedef struct
{
    token_t type;
    char *value;
} token;

typedef struct
{
    const char *str;
    size_t str_i;
    token *current_token;
    token *peek_token;
} lexer;

// Conformorming to the SCL (Shell Command Language) standard
enum token_types
{
    /// @brief Tokens global
    WORD,
    ASSIGNMENT_WORD,
    NAME,
    NEWLINE,
    IO_NUMBER,

    /// @brief Tokens separator
    AND_IF, // &&
    OR_IF, // ||
    DSEMI, // ;;

    /// @brief Tokens redirection
    DLESS, // <<
    DGREAT, // >>
    LESSAND, // <&
    GREATAND, // >&
    LESSGREAT, // <>
    DLESSDASH, // <<-

    /// @brief Token clobber
    CLOBBER, // >|

    /// -----------------
    /// RESERVED WORDS
    /// -----------------

    /// @brief `if` reserved word
    IF, // if
    THEN, // then
    ELSE, // else
    ELIF, // elif
    FI, // fi

    /// @brief `case` reserved word
    CASE, // case
    ESAC, // esac

    /// @brief `while` reserved word
    WHILE, // while
    UNTIL, // until

    /// @brief `do` reserved word
    DO, // do
    DONE, // done

    /// @brief `for` reserved word
    FOR, // for

    /* These are reserved words, not operator tokens, and are
   recognized when reserved words are recognized. */
    LBRACE, // {
    RBRACE, // }
    BANG, // !

    /// @brief `in` reserved word
    IN, // in
};

/**
 * @brief Initialize a lexer with a string
 *
 * @param str The string to tokenize
 * @return lexer* The lexer
 */
lexer *lexer_init(const char *str);

#endif /* LEXER_H */
