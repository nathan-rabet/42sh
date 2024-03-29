#include "../include/ast.h"
#include "../include/xalloc.h"

struct ast *ast_pipe_init(size_t nb_command, struct ast **command)
{
    static struct ast_vtable vtable = {
        .run = &pipe_run,
        .free = &pipe_free,
        .pretty_print = &pipe_pretty_print,
    };
    struct ast_pipe *pipe_ast = xmalloc(1, sizeof(struct ast_pipe));
    pipe_ast->base.type = AST_PIPE;
    pipe_ast->base.vtable = &vtable;
    pipe_ast->nb_command = nb_command;
    pipe_ast->command = command;
    return &pipe_ast->base;
}

int pipe_run(struct ast *ast)
{
    assert(ast && ast->type == AST_PIPE);
    // struct ast_pipe *pipe_ast = (struct ast_pipe *)ast;
    int res = 0;
    return res;
}

void pipe_free(struct ast *ast)
{
    assert(ast && ast->type == AST_PIPE);
    struct ast_pipe *pipe_ast = (struct ast_pipe *)ast;

    if (pipe_ast->command)
    {
        for (size_t i = 0; i < pipe_ast->nb_command; i++)
            pipe_ast->command[i]->vtable->free(pipe_ast->command[i]);
        xfree(pipe_ast->command);
    }
    xfree(ast);
}

void pipe_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_PIPE);
    struct ast_pipe *pipe_ast = (struct ast_pipe *)ast;
    if (pipe_ast->command)
    {
        printf("PIPELINE command : ");

        for (size_t i = 0; i < pipe_ast->nb_command; i++)
        {
            printf(" | ");
            pipe_ast->command[i]->vtable->pretty_print(pipe_ast->command[i]);
        }
        xfree(pipe_ast->command);
    }
}
