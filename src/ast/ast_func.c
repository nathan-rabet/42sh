#include "../include/ast.h"
#include "../include/xalloc.h"


struct ast *ast_func_init(char *name, struct ast *to_execute)
{
    static struct ast_vtable vtable = {
            .run = &func_run,
            .free = &func_free,
            .pretty_print = &func_pretty_print,
    };
    struct ast_func *func_ast = xmalloc(1, sizeof(struct ast_func));
    func_ast->base.type = AST_FUNC;
    func_ast->base.vtable = &vtable;
    func_ast->name = name;
    func_ast->to_execute = to_execute;
    return &func_ast->base;
}

int func_run(struct ast *ast) {
    assert(ast && ast->type == AST_FUNC);
    struct ast_func *func_ast = (struct ast_func *)ast;
    //TODO(clara)
    func_ast->to_execute->vtable->run(func_ast->to_execute);
    return false;
}

void func_free(struct ast *ast)
{
    assert(ast && ast->type == AST_FUNC);
    struct ast_func *func_ast = (struct ast_func*) ast;
    xfree(func_ast->name);
    func_ast->to_execute->vtable->free(func_ast->to_execute);
    xfree(ast);
}

void func_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_FUNC);
    struct ast_func *func_ast = (struct ast_func*) ast;
    printf("func [ name: %s ", func_ast->name);
    printf("do : ");
    func_ast->to_execute->vtable->pretty_print(func_ast->to_execute);
    printf("done func ]\n");
}