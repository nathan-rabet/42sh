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

typedef char param_exp_type_t;

typedef struct _parameter_expansion
{
    param_exp_type_t type;
    char *word;
} parameter_expansion;

typedef char quote_t;

struct token
{
    token_t type;
    quote_t quote;
    void *value;
    struct token *next;
};

typedef struct
{
    const char *str;
    size_t str_token_start;
    size_t str_token_end;
    quote_t quote;
    // TODO(robertnant): potentially remove nb_tokens
    // as tokens will be represented as a linked list.
    size_t nb_tokens;
    struct token *tokens;
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
    IN, // in

    LESS, // <
    GREAT, // >

    /// @brief out of SCL tokens
    SEMI, // ;
    PIPE, // |
    COMMAND // Any executable command ("ls -l", "./42sh")
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
    "<", // LESS
    ">", // GREAT

    ";", // GREAT
    "|", // PIPE

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
    return TOKEN_UNDEFINED;
}

#define IS_RESERVED_WORD(type) (type >= IF && type <= IN)
#define IS_TOKEN_IN_QUOTES(t) (t.is_in_dquote || t.is_in_squote)
#define IS_CURRENT_IN_QUOTES(lexer)                                            \
    (lexer.quote == QUOTE_SINGLE || lexer.quote == QUOTE_DOUBLE)

/**
 * @brief Create tokens from input string
 *
 * @param input The input string
 * @param tokens_len The length of the returned array
 * @return token* The array of tokens
 */
struct token *get_tokens(const char *input, size_t *tokens_len);

#endif /* LEXER_H */
