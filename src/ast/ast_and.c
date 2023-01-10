#include "../../include/ast.h"
#include "../../include/xalloc.h"


struct ast *ast_and_init(struct ast *ast_left, struct ast *ast_right)
{
    static struct ast_vtable vtable = {
            .run = &and_run,
            .free = &and_free,
            .pretty_print = &and_pretty_print,
    };
    struct ast_and *and_ast = xmalloc(1, sizeof(struct ast_and));
    and_ast->base.type = AST_AND;
    and_ast->base.vtable = &vtable;
    and_ast->ast_left = ast_left;
    and_ast->ast_right = ast_right;
    return &and_ast->base;
}

bool and_run(struct ast *ast) {
    assert(ast && ast->type == AST_AND);
    struct ast_and *and_ast = (struct ast_and *)ast;
    return and_ast->ast_left->vtable->run(and_ast->ast_left) &&
        and_ast->ast_right->vtable->run(and_ast->ast_right);
}

void and_free(struct ast *ast)
{
    assert(ast && ast->type == AST_AND);
    struct ast_and *and_ast = (struct ast_and*) ast;
    and_ast->ast_left->vtable->free(and_ast->ast_left);
    and_ast->ast_right->vtable->free(and_ast->ast_right);
    xfree(ast);
}

void and_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_AND);
    struct ast_and *and_ast = (struct ast_and*) ast;
    printf("AND [ ");
    and_ast->ast_left->vtable->pretty_print(and_ast->ast_left);
    printf(", ");
    and_ast->ast_right->vtable->pretty_print(and_ast->ast_right);
    printf("]\n");
}