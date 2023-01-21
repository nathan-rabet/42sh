#include "../include/ast.h"
#include "../include/xalloc.h"


struct ast *ast_while_init(struct ast *condition, struct ast *to_execute)
{
    static struct ast_vtable vtable = {
            .run = &while_run,
            .free = &while_free,
            .pretty_print = &while_pretty_print,
    };
    struct ast_while *while_ast = xmalloc(1, sizeof(struct ast_while));
    while_ast->base.type = AST_WHILE;
    while_ast->base.vtable = &vtable;
    while_ast->condition = condition;
    while_ast->to_execute = to_execute;
    return &while_ast->base;
}

int while_run(struct ast *ast) {
    assert(ast && ast->type == AST_WHILE);
    struct ast_while *while_ast = (struct ast_while *)ast;
    while (while_ast->condition->vtable->run(while_ast->condition))
        while_ast->to_execute->vtable->run(while_ast->to_execute);
    return true;
}

void while_free(struct ast *ast)
{
    assert(ast && ast->type == AST_WHILE);
    struct ast_while *while_ast = (struct ast_while*) ast;
    while_ast->condition->vtable->free(while_ast->condition);
    while_ast->to_execute->vtable->free(while_ast->to_execute);
    xfree(ast);
}

void while_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_WHILE);
    struct ast_while *while_ast = (struct ast_while*) ast;
    printf("while condition : ");
    while_ast->condition->vtable->pretty_print(while_ast->condition);
    printf("do : ");
    while_ast->to_execute->vtable->pretty_print(while_ast->to_execute);
    printf("done\n");
}