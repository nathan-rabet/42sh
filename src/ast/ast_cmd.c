#include "../../include/ast.h"
#include "../../include/xalloc.h"


struct ast *ast_cmd_init(char **word)
{
    static struct ast_vtable vtable =
    {
            .run = &cmd_run,
            .free = &cmd_free,
            .pretty_print = &cmd_pretty_print,
    };
    struct ast_cmd *cmd = xmalloc(1, sizeof(struct ast_cmd));
    cmd->base.type = AST_CMD;
    cmd->base.vtable = &vtable;
    cmd->words = word;
    return &cmd->base;
}

bool cmd_run(struct ast *ast) {
    assert(ast && ast->type == AST_CMD);
   // struct ast_cmd *cmd = (struct ast_cmd *)ast;
    //TODO(clara) run_command:
    // (cmd->words);
    return true;
}

void cmd_free(struct ast *ast)
{
    assert(ast && ast->type == AST_CMD);
    struct ast_cmd *cmd = (struct ast_cmd*) ast;
    if (cmd->words)
    {
        for (size_t i = 0; cmd->words[i];i++)
            xfree(cmd->words[i]);
        xfree(cmd->words);
        xfree(ast);
    }
}

void cmd_pretty_print(struct ast *ast)
{
    assert(ast && ast->type == AST_CMD);
    struct ast_cmd *cmd = (struct ast_cmd*) ast;
    printf("CMD [\n ");
    for (size_t i = 0; cmd->words[i];i++)
        printf("\tWord nb %zu : %s \n", i, cmd->words[i]);
    printf("]\n");
}
