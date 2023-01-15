#include "../../include/ast.h"
#include "../../include/xalloc.h"


struct ast *ast_or_init(struct ast *ast_left, struct ast *ast_right)
{
    static struct ast_vtable vtable = {
            .run = &or_run,
            .free = &or_free,
            .pretty_print = &or_pretty_print,
    };
    struct ast_or *or_ast = xmalloc(1, sizeof(struct ast_or));
    or_ast->base.type = AST_OR;
    or_ast->base.vtable = &vtable;
    or_ast->ast_left = ast_left;
    or_ast->ast_right = ast_right;
    return &or_ast->base;
}

bool or_run(struct ast *ast) {
    assert(ast && ast->type == AST_OR);
    struct ast_or *or_ast = (struct ast_or *)ast;
    return or_ast->ast_left->vtable->run(or_ast->ast_left) ||
           or_ast->ast_right->vtable->run(or_ast->ast_right);
}

void or_free(struct ast *ast)
{
    assert(ast && ast->type == AST_OR);
    struct ast_or *or_ast = (struct ast_or*) ast;
    or_ast->ast_left->vtable->free(or_ast->ast_left);
    or_ast->ast_right->vtable->free(or_ast->ast_right);
    xfree(ast);
}

void or_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_OR);
    struct ast_or *or_ast = (struct ast_or*) ast;
    printf("OR [ LEFT :");
    or_ast->ast_left->vtable->pretty_print(or_ast->ast_left);
    printf(", RIGHT :");
    or_ast->ast_right->vtable->pretty_print(or_ast->ast_right);
    printf("]\n");
}