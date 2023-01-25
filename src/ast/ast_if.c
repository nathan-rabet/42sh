#include "../include/ast.h"
#include "../include/xalloc.h"

struct ast *ast_if_init(struct ast *condition, struct ast *then_body,
                        struct ast *else_body)
{
    static struct ast_vtable vtable = {
        .run = &if_run,
        .free = &if_free,
        .pretty_print = &if_pretty_print,
    };
    struct ast_if *if_ast = xmalloc(1, sizeof(struct ast_if));
    if_ast->base.type = AST_IF;
    if_ast->base.vtable = &vtable;
    if_ast->condition = condition;
    if_ast->then_body = then_body;
    if_ast->else_body = else_body;
    return &if_ast->base;
}

int if_run(struct ast *ast)
{
    assert(ast && ast->type == AST_IF);
    struct ast_if *if_ast = (struct ast_if *)ast;
    if (if_ast->condition)
        return if_ast->then_body->vtable->run(if_ast->then_body);
    else if (if_ast->else_body != NULL)
        return if_ast->else_body->vtable->run(if_ast->else_body);
    return false;
}

void if_free(struct ast *ast)
{
    assert(ast && ast->type == AST_IF);
    struct ast_if *if_ast = (struct ast_if *)ast;
    if_ast->then_body->vtable->free(if_ast->then_body);
    if (if_ast->else_body != NULL)
        if_ast->else_body->vtable->free(if_ast->else_body);
    xfree(ast);
}

void if_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_IF);
    struct ast_if *if_ast = (struct ast_if *)ast;
    printf("IF condition: ");
    if_ast->condition->vtable->pretty_print(if_ast->condition);
    printf("\nTHEN [ ");
    if_ast->then_body->vtable->pretty_print(if_ast->then_body);
    printf(" ]\n");
    if (if_ast->else_body != NULL)
    {
        printf("\nELSE [ ");
        if_ast->else_body->vtable->pretty_print(if_ast->else_body);
        printf("]\n");
    }
}
