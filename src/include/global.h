#ifndef GLOBAL_H
#define GLOBAL_H

#include "ast.h"

typedef struct alias
{
    const char *name;
    const char *value;
    struct alias *next;
} alias;

typedef struct loop_stack
{
    struct ast *current_loop_ast;
    struct ast_loop_stack *next;
} loop_stack;

typedef struct global_sh
{
    alias *alias_ll;
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

#endif /* GLOBAL_H */
