#ifndef GLOBAL_H
#define GLOBAL_H

#include "ast.h"

typedef struct alias
{
    bool is_used;
    char *name;
    char *value;
    struct alias *next;
} alias;

typedef struct variable
{
    char *name;
    char *value;
    struct variable *next;
} variable;

typedef struct loop_stack
{
    struct ast *current_loop_ast;
    struct ast_loop_stack *next;
} loop_stack;

typedef struct global_sh
{
    alias *alias_ll;
    variable *variable;
    struct ast *ast;
    int last_exit_status;
    loop_stack *loop_stack;
} global_sh;

/**
 * @brief Get the global_sh struct
 * @return The global_sh struct
 */
global_sh *get_global_sh(void);

/**
 * @brief Set the global_sh struct
 * @param global_sh The global_sh struct
 */
void push_loop_stack(struct ast *ast);

/**
 * @brief Remove the current loop ast from the stack and return it
 *
 * @return The popped loop ast
 */
struct ast *pop_loop_stack(void);

/**
 *  @brief push variable parameter
 * @param name the name of the variable
 * @param value the value of the variable
 */
void push_variable(char *name, char *value);

/**
 * @brief Get the value of the name variable return NULL if not find
 * @param name
 * @return
 */
char *get_variable(char *name);

/**
 * @brief remove variable
 * @param name
 */
void remove_variable(char *name);

/**
 * @brief update the variable
 * @param name
 * @param value
 */
void update_variable(char *name, char *value);

#endif /* GLOBAL_H */
