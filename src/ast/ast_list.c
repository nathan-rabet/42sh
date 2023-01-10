#include "../../include/ast.h"
#include "../../include/xalloc.h"


struct ast *ast_list_init(size_t nb_children, struct ast **children)
{
    static struct ast_vtable vtable = {
            .run = &list_run,
            .free = &list_free,
            .pretty_print = &list_pretty_print,
    };
    struct ast_list *list_ast = xmalloc(1, sizeof(struct ast_list));
    list_ast->base.type = AST_LIST;
    list_ast->base.vtable = &vtable;
    list_ast->nb_children = nb_children;
    list_ast->children = children;
    return &list_ast->base;
}

void list_run(struct ast *ast) {
    assert(ast && ast->type == AST_LIST);
    struct ast_list *list_ast = (struct ast_list *)ast;
    for (size_t i = 0; i < list_ast->nb_children; i++)
        list_ast->children[i]->vtable->run(list_ast->children[i]);
}

void list_free(struct ast *ast)
{
    assert(ast && ast->type == AST_LIST);
    struct ast_list *list_ast = (struct ast_list*) ast;
    if (list_ast->children)
    {
        for (size_t i = 0; i < list_ast->nb_children;i++)
            list_ast->children[i]->vtable->free(list_ast->children[i]);
        xfree(list_ast->children);
        xfree(ast);
    }
}

void list_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_LIST);
    struct ast_list *list_ast = (struct ast_list*) ast;
    printf("LIST : \n");
    for (size_t i = 0; i < list_ast->nb_children;i++)
    {
        printf("\tChildren %zu", i);
        list_ast->children[i]->vtable->pretty_print(list_ast->children[i]);
    }
}

