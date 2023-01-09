#include "../../include/ast.h"
#include "../../include/xalloc.h"


struct ast *ast_cmd_init(char **word){
    static struct ast_vtable vtable = {
            .run = &cmd_run,
            .free = &cmd_free;
            .pretty_print = &cmd_pretty_print,
    };
    struct ast_cmd *cmd = xmalloc(1, sizeof(struct ast_cmd));
    cmd->base.type = AST_CMD;
    cmd->base.vtable = &vtable;
    cmd->words = word;
    return &cmd->base;
}
