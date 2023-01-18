#ifndef EXECUTION_H
#define EXECUTION_H

#include "ast.h"
#include "xalloc.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

struct dup_item
{
    int fd;
    int fd_dup;
    struct dup_item *next;
};

/**
 * @brief list of dup item
 */
struct list_dup
{
    struct dup_item *head; // head of the list
};

int exec_builtins(char **argv, int *error);
int exec_cmd(char **argv, int *error);

// Utils
bool is_builtin(char *name);

#endif //EXECUTION_H
