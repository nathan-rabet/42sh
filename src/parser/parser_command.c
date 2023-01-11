// GRAMMAR
// command =
//simple_command
//| shell_command { redirection }
//| funcdec { redirection }
//;

#include "../../include/parser.h"

struct ast *parser_command(token *current_token)
{
    struct ast *ast = NULL;
}
