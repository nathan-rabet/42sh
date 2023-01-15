#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>

typedef int token_t;

enum token_types
{
    QUOTE_NONE = 0,
    QUOTE_SINGLE,
    QUOTE_DOUBLE,
    QUOTE_BACK
};

typedef char quote_t;

typedef struct token
{
    token_t type;
    quote_t quote;
    void *value;
    struct token *next;
} token;

typedef struct
{
    const char *str;
    char *str_token_start;
    char *str_token_end;
    quote_t quote;
    token *tokens;
} lexer;

// Conformorming to the SCL (Shell Command Language) standard
enum token_types
{
    TOKEN_UNDEFINED = 0, // Undefined token
    /// @brief Tokens global
    WORD, // Any word
    IO_NUMBER, // /[<>]\d+/
    NEWLINE, // \n

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

    LESS, // <
    GREAT, // >

    //// @brief out of SCL tokens
    SEMI, // ;
    PIPE, // |
};

const char *tokens_mapping = {
    NULL, // UNDEFINED
    NULL, // WORD
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
};

// Tokens utils
#define FIRST_HARD_CODED NEWLINE
#define LAST_HARD_CODED PIPE

// Operators utils
#define FIRST_OPERATOR AND_IF
#define LAST_OPERATOR CLOBBER

/**
 * @brief Create tokens from input string
 *
 * @param input The input string
 * @return token* The array of tokens
 */
token *get_tokens(const char *input);

/**
 * @brief Lex the input string which starts with a backslash
 *
 * @note str must start with a backslash
 *
 * @param tokens The tokens
 * @param str The input string
 * @return char* The string after the backslash
 */
char *lexer_eat_backslash(token_t tokens, char *str);

/**
 * @brief Lex the input string which starts with a single quote or a double
 * quote
 *
 * @note str must start with a single quote or a double quote
 *
 * @param tokens The tokens
 * @param str The input string
 * @return char* The string after the single quote
 */
char *lexer_eat_quotes(token_t tokens, char *str);

/**
 * @brief Tells which token type is the prefix of the input string
 * @param lex The lexer structure
 * @param pre_size The size of the prefix
 *
 * @return token_t The prefix token type
 */
token_t lexer_is_token_prefix(lexer *lex, size_t pre_size);

#endif /* LEXER_H */
