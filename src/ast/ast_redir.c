#include "../include/ast.h"
#include "../include/xalloc.h"

struct ast *ast_redir_init(struct list_redir *list, struct ast *command)
{
    static struct ast_vtable vtable = {
        .run = &redir_run,
        .free = &redir_free,
        .pretty_print = &redir_pretty_print,
    };
    struct ast_redir *redir_ast = xmalloc(1, sizeof(struct ast_redir));
    redir_ast->base.type = AST_REDIR;
    redir_ast->base.vtable = &vtable;
    redir_ast->command = command;
    redir_ast->list = list;
    return &redir_ast->base;
}

struct list_redir *list_redir_init(enum ast_redir_type type, char *IONumber,
                                   char *target, struct list_redir *next)
{
    struct list_redir *list = xmalloc(1, sizeof(struct list_redir));
    list->type = type;
    list->IONumber = IONumber;
    list->target = target;
    list->next = next;
    return list;
}

int redir_run(struct ast *ast)
{
    assert(ast && ast->type == AST_REDIR);
    struct ast_redir *redir_ast = (struct ast_redir *)ast;
    exec_redir(redir_ast->list);
    return redir_ast->command->vtable->run(redir_ast->command);
}

void redir_free(struct ast *ast)
{
    assert(ast && ast->type == AST_REDIR);
    // struct ast_redir *redir_ast = (struct ast_redir*) ast;
    xfree(ast);
}

void redir_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_REDIR);
    struct ast_redir *redir_ast = (struct ast_redir *)ast;
    struct list_redir *list = redir_ast->list;
    printf("REDIRECTION LIST [ COMMAND :");
    redir_ast->command->vtable->pretty_print(redir_ast->command);
    printf("LIST : ");
    while (list != NULL)
    {
        printf(" type : %i, IOnumber : %s, target %s | ", list->type,
               list->IONumber, list->target);
        list = list->next;
    }
    printf("] \n");
}
