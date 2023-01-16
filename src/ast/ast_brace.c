#include "../../include/ast.h"
#include "../../include/xalloc.h"


struct ast *ast_brace_init(struct ast *command)
{
    static struct ast_vtable vtable = {
            .run = &brace_run,
            .free = &brace_free,
            .pretty_print = &brace_pretty_print,
    };
    struct ast_brace *brace_ast = xmalloc(1, sizeof(struct ast_brace));
    brace_ast->base.type = AST_BRACE;
    brace_ast->base.vtable = &vtable;
    brace_ast->command = command;
    return &brace_ast->base;
}

bool brace_run(struct ast *ast) {
    assert(ast && ast->type == AST_BRACE);
    struct ast_brace *brace_ast = (struct ast_brace *)ast;
    return brace_ast->command->vtable->run(brace_ast->command);
}

void brace_free(struct ast *ast)
{
    assert(ast && ast->type == AST_BRACE);
    struct ast_brace *brace_ast = (struct ast_brace*) ast;
    brace_ast->command->vtable->free(brace_ast->command);
    xfree(ast);
}

void brace_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_BRACE);
    struct ast_brace *brace_ast = (struct ast_brace*) ast;
    printf("BRACE [ ");
    brace_ast->command->vtable->pretty_print(brace_ast->command);
    printf("]\n");
}