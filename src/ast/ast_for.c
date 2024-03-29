#include "../include/ast.h"
#include "../include/xalloc.h"

struct ast *ast_for_init(char *name, struct ast *word, struct ast *to_execute)
{
    static struct ast_vtable vtable = {
        .run = &for_run,
        .free = &for_free,
        .pretty_print = &for_pretty_print,
    };
    struct ast_for *for_ast = xmalloc(1, sizeof(struct ast_for));
    for_ast->base.type = AST_FOR;
    for_ast->base.vtable = &vtable;
    for_ast->name = name;
    for_ast->word = word;
    for_ast->to_execute = to_execute;
    return &for_ast->base;
}

int for_run(struct ast *ast)
{
    assert(ast && ast->type == AST_FOR);
    int status = 0;
    struct ast_for *for_ast = (struct ast_for *)ast;
    struct ast_cmd *exec_word = (struct ast_cmd *)for_ast->word;
    if (exec_word == NULL)
    {
        status = for_ast->to_execute->vtable->run(for_ast->to_execute);
        return status;
    }
    char **word = exec_word->words;
    push_variable(for_ast->name, NULL);
    for (size_t i = 0; word[i]; i++)
    {
        update_variable(for_ast->name, word[i]);
        status = for_ast->to_execute->vtable->run(for_ast->to_execute);
    }
    remove_variable(for_ast->name);
    return status;
}

void for_free(struct ast *ast)
{
    assert(ast && ast->type == AST_FOR);
    struct ast_for *for_ast = (struct ast_for *)ast;
    xfree(for_ast->name);
    for_ast->word->vtable->free(for_ast->word);
    for_ast->to_execute->vtable->free(for_ast->to_execute);
    xfree(ast);
}

void for_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_FOR);
    struct ast_for *for_ast = (struct ast_for *)ast;
    printf("FOR name: %s in ", for_ast->name);
    if (for_ast->word != NULL)
        for_ast->word->vtable->pretty_print(for_ast->word);
    printf("do : ");
    for_ast->to_execute->vtable->pretty_print(for_ast->to_execute);
    printf("done\n");
}
