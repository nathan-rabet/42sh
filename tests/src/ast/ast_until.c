#include "../include/ast.h"
#include "../include/xalloc.h"

struct ast *ast_until_init(struct ast *condition, struct ast *to_execute)
{
    static struct ast_vtable vtable = {
        .run = &until_run,
        .free = &until_free,
        .pretty_print = &until_pretty_print,
    };
    struct ast_until *until_ast = xmalloc(1, sizeof(struct ast_until));
    until_ast->base.type = AST_UNTIL;
    until_ast->base.vtable = &vtable;
    until_ast->condition = condition;
    until_ast->to_execute = to_execute;
    return &until_ast->base;
}

int until_run(struct ast *ast)
{
    assert(ast && ast->type == AST_UNTIL);
    struct ast_until *until_ast = (struct ast_until *)ast;
    int status = 0;
    do
        status = until_ast->to_execute->vtable->run(until_ast->to_execute);
    while (until_ast->condition->vtable->run(until_ast->condition) == 0);
    return status;
}

void until_free(struct ast *ast)
{
    assert(ast && ast->type == AST_UNTIL);
    struct ast_until *until_ast = (struct ast_until *)ast;
    until_ast->condition->vtable->free(until_ast->condition);
    until_ast->to_execute->vtable->free(until_ast->to_execute);
    xfree(ast);
}

void until_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_UNTIL);
    struct ast_until *until_ast = (struct ast_until *)ast;
    printf("until condition : ");
    until_ast->condition->vtable->pretty_print(until_ast->condition);
    printf("do : ");
    until_ast->to_execute->vtable->pretty_print(until_ast->to_execute);
    printf("done\n");
}