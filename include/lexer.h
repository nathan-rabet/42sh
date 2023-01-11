#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef int token_t;

typedef struct
{
    token_t type;
    bool is_in_dquote;
    bool is_in_squote;
    void *value;
    struct token *next;
} token;

typedef struct
{
    const char *str;
    size_t value_start;
    size_t value_end;
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
};

#define FIRST_OPERATOR AND_IF
#define LAST_OPERATOR CLOBBER
#define IS_OPERATOR(type) (type >= FIRST_OPERATOR && type <= LAST_OPERATOR)
#define IS_RESERVED_WORD(type) (type >= IF && type <= IN)
#define IS_IN_QUOTES(t) (t.is_in_dquote || t.is_in_squote)

/**
 * @brief Initialize a lexer with a string
 *
 * @param str The string to tokenize
 * @return lexer* The lexer
 */
lexer *lexer_init(const char *str);

#endif /* LEXER_H */
