#include "../../include/ast.h"

const char *ast_representation(enum ast_type type)
{
    static const char *const names[] = {
            [AST_CMD] = "CMD",
            [AST_IF] = "IF",
            [AST_LIST] = "LIST",
    };
    return names[type];
}