#include "../include/ast.h"
#include "../include/xalloc.h"


struct ast *ast_subshell_init(struct ast *command)
{
    static struct ast_vtable vtable = {
            .run = &subshell_run,
            .free = &subshell_free,
            .pretty_print = &subshell_pretty_print,
    };
    struct ast_subshell *subshell_ast = xmalloc(1, sizeof(struct ast_subshell));
    subshell_ast->base.type = AST_SUBSHELL;
    subshell_ast->base.vtable = &vtable;
    subshell_ast->command = command;
    return &subshell_ast->base;
}

int subshell_run(struct ast *ast) {
    assert(ast && ast->type == AST_SUBSHELL);
    struct ast_subshell *subshell_ast = (struct ast_subshell *)ast;
    return subshell_ast->command->vtable->run(subshell_ast->command);
}

void subshell_free(struct ast *ast)
{
    assert(ast && ast->type == AST_SUBSHELL);
    struct ast_subshell *subshell_ast = (struct ast_subshell*) ast;
    subshell_ast->command->vtable->free(subshell_ast->command);
    xfree(ast);
}

void subshell_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_SUBSHELL);
    struct ast_subshell *subshell_ast = (struct ast_subshell*) ast;
    printf("SUBSHELL [ ");
    subshell_ast->command->vtable->pretty_print(subshell_ast->command);
    printf("]\n");
}
