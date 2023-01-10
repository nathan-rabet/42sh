#include "../../include/ast.h"
#include "../../include/xalloc.h"


struct ast *ast_redir_init(enum ast_redir_type type,char *IONumber, char *target)
{
    static struct ast_vtable vtable = {
            .run = &redir_run,
            .free = &redir_free,
            .pretty_print = &redir_pretty_print,
    };
    struct ast_redir *redir_ast = xmalloc(1, sizeof(struct ast_redir));
    redir_ast->base.type = AST_REDIR;
    redir_ast->base.vtable = &vtable;
    redir_ast->type = type;
    redir_ast->IONumber = IONumber;
    redir_ast->target = target;
    return &redir_ast->base;
}

bool redir_run(struct ast *ast) {
    assert(ast && ast->type == AST_REDIR);
    struct ast_redir *redir_ast = (struct ast_redir *)ast;
    //TODO(clara) execution redirection
}

void redir_free(struct ast *ast)
{
    assert(ast && ast->type == AST_REDIR);
    struct ast_redir *redir_ast = (struct ast_redir*) ast;
    xfree(redir_ast->IONumber);
    xfree(redir_ast->target);
    xfree(ast);
}

void redir_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_REDIR);
    struct ast_redir *redir_ast = (struct ast_redir*) ast;
    printf("REDIRECTION [ type : %i, IOnumber : %s, target %s \n",
           redir_ast->type, redir_ast->IONumber, redir_ast->target);

}