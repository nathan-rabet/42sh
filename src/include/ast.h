#ifndef AST_H
#define AST_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "execution.h"
#include "global.h"

/**
 * @brief Node of the AST
 */
enum ast_type
{
    AST_CMD,
    AST_IF,
    AST_LIST,
    AST_REDIR,
    AST_AND,
    AST_OR,
    AST_NOT,
    AST_WHILE,
    AST_UNTIL,
    AST_FOR,
    AST_FUNC,
    AST_CASE,
    AST_PIPE,
    AST_BRACE,
    AST_SUBSHELL,
};

struct ast_vtable;

/**
 * @brief Structur of the ast
 * @param type the type of the node
 * @param vtable the different available functions
 */
struct ast
{
    enum ast_type type;
    struct ast_vtable *vtable;
};

/**
 * @brief the different available functions
 * @param run the run function
 * @param free the free function
 * @param pretty_print the print function
 */
struct ast_vtable
{
    int (*run)(struct ast *ast);
    void (*free)(struct ast *ast);
    void (*pretty_print)(struct ast *ast);
};

/**
 * @brief the cmd node
 * @param base the linked of the ast structure
 * @param word the linked list of instruction
 */
struct ast_cmd
{
    struct ast base;
    char **words; // NULL terminated char* list
};

struct ast *ast_cmd_init(char **word);
int cmd_run(struct ast *ast);
void cmd_free(struct ast *ast);
void cmd_pretty_print(struct ast *ast);

/**
 * @brief the if node
 * @param base the linked of the ast structure
 * @param condition the condition of the if
 * @param then_body if the condition is true
 * @param else_body if the condition is false, may be NULL
 */
struct ast_if
{
    struct ast base;
    struct ast *condition;
    struct ast *then_body;
    struct ast *else_body;
};

struct ast *ast_if_init(struct ast *condition, struct ast *then_body,
                        struct ast *else_body);
int if_run(struct ast *ast);
void if_free(struct ast *ast);
void if_pretty_print(struct ast *ast);

/**
 * @brief the list node
 * @param base the linked of the ast structure
 * @param nb_children number of children of the children arg
 * @param children the list of children
 */
struct ast_list
{
    struct ast base;
    size_t nb_children;
    struct ast **children;
};

struct ast *ast_list_init(size_t nb_children, struct ast **children);
int list_run(struct ast *ast);
void list_free(struct ast *ast);
void list_pretty_print(struct ast *ast);

/**
 * @brief the different type of redirection
 */
enum ast_redir_type
{
    /// @brief Tokens redirection
    REDIR_LESS, // < redirecting input
    REDIR_GREAT, // > redirecting output no clobber
    REDIR_DLESS, // << here document
    REDIR_DGREAT, // >> Appending redirection output
    REDIR_LESSAND, // <& Duplicate an input file descriptor
    REDIR_GREATAND, // >& Duplicate an output file descriptor
    REDIR_LESSGREAT, // <> open file descriptor for reading and writing
    REDIR_DLESSDASH, // <<- here document

    /// @brief Token clobber
    REDIR_CLOBBER, // >| redirecting output

};

/**
 * @brief the redir item
 * @param type the type of redirection
 * @param IONumber the ionumber
 * @param target the name of the target file of the redirection
 */
struct list_redir
{
    enum ast_redir_type type;
    char *IONumber;
    char *target;
    struct list_redir *next;
};

/**
 * @brief the redir node
 * @param base the linked of the ast structure
 * @param command the command of what we display
 * @param list of the all the redirection
 */
struct ast_redir
{
    struct ast base;
    struct ast *command;
    struct list_redir *list;
};

struct ast *ast_redir_init(struct list_redir *redir, struct ast *command);
struct list_redir *list_redir_init(enum ast_redir_type type, char *IONumber,
                                   char *target, struct list_redir *next);
int redir_run(struct ast *ast);
void redir_free(struct ast *ast);
void redir_pretty_print(struct ast *ast);

/**
 * @brief the and node
 * @param base the linked of the ast structure
 * @param ast_left the left side of the and
 * @param ast_right the right side of the and
 */
struct ast_and
{
    struct ast base;
    struct ast *ast_left;
    struct ast *ast_right;
};

struct ast *ast_and_init(struct ast *ast_left, struct ast *ast_right);
int and_run(struct ast *ast);
void and_free(struct ast *ast);
void and_pretty_print(struct ast *ast);

/**
 * @brief the or node
 * @param base the linked of the ast structure
 * @param ast_left the left side of the or
 * @param ast_right the right side of the or
 */
struct ast_or
{
    struct ast base;
    struct ast *ast_left;
    struct ast *ast_right;
};

struct ast *ast_or_init(struct ast *ast_left, struct ast *ast_right);
int or_run(struct ast *ast);
void or_free(struct ast *ast);
void or_pretty_print(struct ast *ast);

/**
 * @brief the not node
 * @param base the linked of the ast structure
 * @param ast_not the ast where the not is apply
 */
struct ast_not
{
    struct ast base;
    struct ast *ast_not;
};

struct ast *ast_not_init(struct ast *ast_not);
int not_run(struct ast *ast);
void not_free(struct ast *ast);
void not_pretty_print(struct ast *ast);

/**
 * @brief the while node
 * @param base the linked of the ast structure
 * @param condition the condition
 * @param to_execute the execution part of the while
 */
struct ast_while
{
    struct ast base;
    struct ast *condition;
    struct ast *to_execute;
};

struct ast *ast_while_init(struct ast *condition, struct ast *to_execute);
int while_run(struct ast *ast);
void while_free(struct ast *ast);
void while_pretty_print(struct ast *ast);

/**
 * @brief the until node
 * @param base the linked of the ast structure
 * @param condition the condition
 * @param to_execute the execution part of the until
 */
struct ast_until
{
    struct ast base;
    struct ast *condition;
    struct ast *to_execute;
};

struct ast *ast_until_init(struct ast *condition, struct ast *to_execute);
int until_run(struct ast *ast);
void until_free(struct ast *ast);
void until_pretty_print(struct ast *ast);

/**
 * @brief the for node
 * @param base the linked of the ast structure
 * @param name the name of the variable
 * @param word the list of word that will be the condition
 * @param to_execute the execution part of the for
 */
struct ast_for
{
    struct ast base;
    char *name;
    struct ast *word;
    struct ast *to_execute;
};

struct ast *ast_for_init(char *name, struct ast *word, struct ast *to_execute);
int for_run(struct ast *ast);
void for_free(struct ast *ast);
void for_pretty_print(struct ast *ast);

/**
 * @brief the func node
 * @param base the linked of the ast structure
 * @param name the name of the function
 * @param to_execute the execution part of the function
 */
struct ast_func
{
    struct ast base;
    char *name;
    struct ast *to_execute;
};

struct ast *ast_func_init(char *name, struct ast *to_execute);
int func_run(struct ast *ast);
void func_free(struct ast *ast);
void func_pretty_print(struct ast *ast);

/**
 * @brief list of case item
 * @param word list of matching word
 * @param nb_word number of matching word
 * @param to_execute the ast to execute if the word match
 */
struct list_case_item
{
    char **word;
    size_t nb_word;
    struct ast *to_execute;
};

struct list_case_item *list_case_item_init(char **word, size_t nb_word,
                                           struct ast *to_execute);

/**
 * @brief the case node
 * @param base the linked of the ast structure
 * @param name the name of the variable that will be compare
 * @param case_clause clauses where the name will be compare to
 */
struct ast_case
{
    struct ast base;
    char *name;
    size_t nb_item;
    struct list_case_item **case_item;
};

struct ast *ast_case_init(char *name, struct list_case_item **case_item,
                          size_t nb_item);
int case_run(struct ast *ast);
void case_free(struct ast *ast);
void case_pretty_print(struct ast *ast);

/**
 * @brief the pipe node
 * @param base the linked of the ast structure
 * @param nb_command the number of command to run
 * @param command the ast node to run
 */
struct ast_pipe
{
    struct ast base;
    size_t nb_command;
    struct ast **command;
};

struct ast *ast_pipe_init(size_t nb_command, struct ast **command);
int pipe_run(struct ast *ast);
void pipe_free(struct ast *ast);
void pipe_pretty_print(struct ast *ast);

/**
 * @brief the pipe node
 * @param base the linked of the ast structure
 * @param command the ast node to run
 */
struct ast_brace
{
    struct ast base;
    struct ast *command;
};

struct ast *ast_brace_init(struct ast *command);
int brace_run(struct ast *ast);
void brace_free(struct ast *ast);
void brace_pretty_print(struct ast *ast);

/**
 * @brief the pipe node
 * @param base the linked of the ast structure
 * @param command the ast node to run
 */
struct ast_subshell
{
    struct ast base;
    struct ast *command;
};

struct ast *ast_subshell_init(struct ast *command);
int subshell_run(struct ast *ast);
void subshell_free(struct ast *ast);
void subshell_pretty_print(struct ast *ast);

#endif // AST_H
