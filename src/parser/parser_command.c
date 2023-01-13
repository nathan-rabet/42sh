// GRAMMAR
// command =
//simple_command
//| shell_command { redirection }
//| funcdec { redirection }
//;

#include "../../include/parser.h"

struct ast *parser_command(struct token *current_token)
{
    struct ast *ast = NULL;
    ast = parser_simple_command(current_token);
    return ast;
}
