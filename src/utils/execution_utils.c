#include "../include/execution.h"

bool is_builtin(char *name)
{
    if (strcmp(name, "true") == 0 || strcmp(name, "false") == 0
        || strcmp(name, "echo") == 0 || strcmp(name, "exit") == 0
        || strcmp(name, "cd") == 0 || strcmp(name, "export") == 0
        || *name == '.' || strcmp(name, "unset") == 0
        || strcmp(name, "alias") == 0 || strcmp(name, "unalias") == 0)
        return true;
    return false;
}

void return_and_free(char *msg, int exit_code)
{
    fprintf(stderr, "42sh %s\n", msg);
    close_all_list_dup();
    xfree_all();
    exit(exit_code);
}