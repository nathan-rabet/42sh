#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>

typedef int token_t;

typedef struct token
{
    token_t type;

    char *value;
    struct token *next;
} token;

typedef struct lexer
{
    char *input;
    size_t input_len;
    char *str_token_start;
    char *str_token_end;
    token *tokens;
} lexer;

// Conformorming to the SCL (Shell Command Language) standard
enum token_types
{
    TOKEN_UNDEFINED = 0, // Undefined token
    /// @brief Tokens global
    WORD, // Any word
    // NAME, // /[a-zA-Z_][a-zA-Z0-9_]*/
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
    AMPERSAND, // &
};

// Lexer utils
#define IS_BLANK(c) (c == ' ' || c == '\t')

#define GET_CURRENT_CHAR_ADDR(lex) (lex->str_token_end - 1)
#define GET_LEN_CURRENT_CHAR(lex)                                              \
    ((size_t)(GET_CURRENT_CHAR_ADDR(lex) - lex->str_token_start + 1))
#define GET_CURRENT_CHAR(lex) (*(GET_CURRENT_CHAR_ADDR(lex)))

#define GET_PREVIOUS_CHAR_ADDR(lex) (GET_CURRENT_CHAR_ADDR(lex) - 1)
#define GET_LEN_PREVIOUS_CHAR(lex) (GET_LEN_CURRENT_CHAR(lex) - 1)
#define GET_PREVIOUS_CHAR(lex) (*(GET_PREVIOUS_CHAR_ADDR(lex)))

#define GET_NEXT_CHAR_ADDR(lex) (GET_CURRENT_CHAR_ADDR(lex) + 1)
#define GET_LEN_NEXT_CHAR(lex) (GET_LEN_CURRENT_CHAR(lex) + 1)
#define GET_NEXT_CHAR(lex) (*(GET_NEXT_CHAR_ADDR(lex)))

#define HAS_PREVIOUS_CHAR(lex)                                                 \
    (GET_PREVIOUS_CHAR_ADDR(lex) >= lex->str_token_start)
#define IS_END_OF_INPUT(lex)                                                   \
    (GET_CURRENT_CHAR_ADDR(lex) >= lex->input + lex->input_len)
#define HAS_NEXT_CHAR(lex)                                                     \
    (GET_NEXT_CHAR_ADDR(lex) < lex->input + lex->input_len)

#define CAN_EAT_NEWLINE_JOINING(lex)                                           \
    (GET_CURRENT_CHAR(lex) == '\\' && HAS_NEXT_CHAR(lex)                       \
     && GET_NEXT_CHAR(lex) == '\n')

// Tokens utils
#define FIRST_HARD_CODED NEWLINE
#define LAST_HARD_CODED AMPERSAND

// Operators utils
#define FIRST_OPERATOR AND_IF
#define LAST_OPERATOR CLOBBER
#define IS_OPERATOR(token_type)                                                \
    (token_type >= FIRST_OPERATOR && token_type <= LAST_OPERATOR)

/**
 * @brief Create tokens from input string
 *
 * @param input The input string
 * @param len The length of the input string
 * @return token* The linked-list of tokens
 */
token *get_tokens(const char *input, size_t len);

/**
 * @brief Add a token to the tokens linked-list
 *
 * @param lex The lexer structure pointer
 * @param token_type The type of the token
 * @param value_size The size of the token value
 */
void token_add(lexer *lex, token_t token_type, size_t value_size);

/**
 * @brief Tells if the input string is a name
 *
 * @note A name is a word consisting only of alphanumeric characters and
 * underscores, and beginning with an alphabetic character or an underscore.
 * Also referred to as an identifier.
 *
 * @param str The input string
 * @param len The length of the input string
 * @return true
 * @return false
 */
bool is_name(const char *str, size_t len);

/**
 * @brief Tells if the character is a separator
 *
 * @note A separator is a character that separates words. It is one of
 * the following: || & && ; ;; ;& ;;& ( ) | |& <newline>
 *
 * @param c The input character
 * @return true
 * @return false
 */
bool is_separator(char c);

/**
 * @brief Telling which token type is the input string
 *
 * @param lex The lexer structure
 * @param len The length of the token string
 * @return token_t The token type, or TOKEN_UNDEFINED if not found
 */
token_t lexer_is_token(lexer *lex, size_t len);

/**
 * @brief Tells which operator type is the prefix of the input string
 * @param lex The lexer structure
 * @param len The length of the token string
 *
 * @return token_t The prefix token type, or TOKEN_UNDEFINED if not found
 */
token_t lexer_is_token_prefix_operator(lexer *lex, size_t len);

/**
 * @brief Tells which token type is the prefix of the input string
 * @param lex The lexer structure
 * @param len The length of the token string
 *
 * @return token_t The prefix token type, or TOKEN_UNDEFINED if not found
 */
token_t lexer_is_token_prefix(lexer *lex, size_t len);

/**
 * @brief Performs a newline joining to the input string
 *
 * @note str must start with a backslash
 *
 * @param lex The lexer structure pointer
 */
void lexer_eat_newline_joining(lexer *lex);

/**
 * @brief Lex the input string which starts with a backslash
 *
 * @note str must start with a backslash
 *
 * @param lex The lexer structure pointer
 */
void lexer_eat_backslash(lexer *lex);

/**
 * @brief Lex the input string which starts with a single quote or a double
 * ('...' or "...")
 *
 * @note str must start with a single quote or a double quote
 *
 * @param lex The lexer structure pointer
 */
void lexer_eat_quotes(lexer *lex);

/**
 * @brief Lex the input string with arithmetics expansion (with $((...)))
 *
 * @param lex The lexer structure pointer
 */
void lexer_eat_arithmetics_expansion(lexer *lex);

/**
 * @brief Lex the input string with parameter expansion (with $...)
 *
 * @param lex The lexer structure pointer
 */
void lexer_eat_parameter_expansion(lexer *lex);

/**
 * @brief Lex the input string with parameter expansion (with ${...})
 *
 * @param lex The lexer structure pointer
 */
void lexer_eat_parameter_expansion_braces(lexer *lex);

/**
 * @brief Lex the input string with command substitution (with $(...))
 *
 * @param lex The lexer structure pointer
 */
void lexer_eat_command_substitution(lexer *lex);

/**
 * @brief Lex the input string with command substitution (with `...`)
 *
 * @param lex The lexer structure pointer
 */
void lexer_eat_command_backquote(lexer *lex);

/**
 * @brief Lex the input string with a comment (with #...)
 *
 * @param lex The lexer structure pointer
 */
void lexer_eat_comment(lexer *lex);

#endif /* LEXER_H */
