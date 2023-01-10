#include "../../include/ast.h"
#include "../../include/xalloc.h"


struct ast *ast_not_init(struct ast *ast_not)
{
    static struct ast_vtable vtable = {
            .run = &not_run,
            .free = &not_free,
            .pretty_print = &not_pretty_print,
    };
    struct ast_not *not_ast = xmalloc(1, sizeof(struct ast_not));
    not_ast->base.type = AST_NOT;
    not_ast->base.vtable = &vtable;
    not_ast->ast_not = ast_not;
    return &not_ast->base;
}

bool not_run(struct ast *ast) {
    assert(ast && ast->type == AST_NOT);
    struct ast_not *not_ast = (struct ast_not *)ast;
    return ! not_ast->ast_not->vtable->run(not_ast->ast_not);
}

void not_free(struct ast *ast)
{
    assert(ast && ast->type == AST_NOT);
    struct ast_not *not_ast = (struct ast_not*) ast;
    not_ast->ast_not->vtable->free(not_ast->ast_not);
    xfree(ast);
}

void not_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_NOT);
    struct ast_not *not_ast = (struct ast_not*) ast;
    printf("NOT [ ");
    not_ast->ast_not->vtable->pretty_print(not_ast->ast_not);
    printf("]\n");
}