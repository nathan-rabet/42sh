#ifndef EXECUTION_H
#define EXECUTION_H

#include "ast.h"
#include "xalloc.h"

struct dup_item
{
    int fd;
    struct dup_item *next;
};

/**
 * @brief list of dup item
 */
struct list_dup
{
    struct dup_item *head; // head of the list
};

int exec_cmd(struct ast *ast, int *error);

// Utils
bool is_builtin(char *name);

#endif //EXECUTION_H
