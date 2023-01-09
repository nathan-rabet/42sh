#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef int token_t;

typedef struct
{
    token_t type;
    char *value;
} token;

#define LEXER_TOKENS_BUF_SIZE 4
typedef struct
{
    const char *str;
    size_t str_i;
    token last_tokens[LEXER_TOKENS_BUF_SIZE];
    size_t last_tokens_i;
} lexer;

extern char *IFS;

// Conformorming to the SCL (Shell Command Language) standard
enum token_types
{
    UNDEFINED = 0, // Undefined token
    /// @brief Tokens global
    WORD, // Any word
    ASSIGNMENT_WORD, // Any word that is an assignment
    NAME, // Any word that is a name
    NEWLINE, // \n
    IO_NUMBER, // /[<>]\d+/

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
