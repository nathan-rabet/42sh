#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>

typedef int token_t;

typedef struct
{
    token_t type;
    bool is_in_dquote;
    bool is_in_squote;
    void *value;
} token;

typedef struct
{
    const char *str;
    size_t str_token_start;
    size_t str_token_end;
    bool str_is_in_dquote;
    bool str_is_in_squote;
    size_t nb_tokens;
    token *tokens;
} lexer;

extern char *IFS;

// Conformorming to the SCL (Shell Command Language) standard
enum token_types
{
    TOKEN_UNDEFINED = 0, // Undefined token
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
    IN // in
};

char *tokens_mapping = {
    NULL, // UNDEFINED
    NULL, // WORD
    NULL, // ASSIGNMENT_WORD
    NULL, // NAME
    "\n", // NEWLINE
    NULL, // IO_NUMBER

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
};

// Operators utils
#define FIRST_OPERATOR AND_IF
#define LAST_OPERATOR CLOBBER
#define IS_TOKEN_OPERATOR(token)                                               \
    (toekn.type >= FIRST_OPERATOR && token.type <= LAST_OPERATOR)

inline token_t get_string_operator(char *str, size_t len)
{
    for (int i = FIRST_OPERATOR; i <= LAST_OPERATOR; i++)
        if (strncmp(str, tokens_mapping[i], len) == 0)
            return i;
    return TOKEN_ERROR;
}

#define IS_RESERVED_WORD(type) (type >= IF && type <= IN)
#define IS_TOKEN_IN_QUOTES(t) (t.is_in_dquote || t.is_in_squote)
#define IS_CURRENT_IN_QUOTES(lexer)                                            \
    (lexer.str_is_in_dquote || lexer.str_is_in_squote)

/**
 * @brief Create tokens from input string
 *
 * @param input The input string
 * @param tokens_len The length of the returned array
 * @return token* The array of tokens
 */
token *get_tokens(const char *input, size_t *tokens_len);

#endif /* LEXER_H */
