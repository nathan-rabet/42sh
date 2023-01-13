#include "../../include/ast.h"
#include "../../include/xalloc.h"


struct ast *ast_pipe_init(struct ast *command)
{
    static struct ast_vtable vtable = {
            .run = &pipe_run,
            .free = &pipe_free,
            .pretty_print = &pipe_pretty_print,
    };
    struct ast_pipe *pipe_ast = xmalloc(1, sizeof(struct ast_pipe));
    pipe_ast->base.type = AST_PIPE;
    pipe_ast->base.vtable = &vtable;
    pipe_ast->command = command;
    return &pipe_ast->base;
}

bool pipe_run(struct ast *ast) {
    assert(ast && ast->type == AST_PIPE);
    //struct ast_pipe *pipe_ast = (struct ast_pipe *)ast;
    //TODO(clara)
    return false;
}

void pipe_free(struct ast *ast)
{
    assert(ast && ast->type == AST_PIPE);
    struct ast_pipe *pipe_ast = (struct ast_pipe*) ast;
    pipe_ast->command->vtable->free(pipe_ast->command);
    xfree(ast);
}

void pipe_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_PIPE);
    struct ast_pipe *pipe_ast = (struct ast_pipe*) ast;
    printf("pipe command : ");
    pipe_ast->command->vtable->pretty_print(pipe_ast->command);
}