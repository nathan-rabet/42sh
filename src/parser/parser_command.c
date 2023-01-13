// GRAMMAR
// command =
//simple_command
//| shell_command { redirection }
//| funcdec { redirection }
//;

#include "../../include/parser.h"

bool is_shell_command(struct token *token)
{
    switch (token->type) {
        case IF:
            return true;
        default:
            return false;
    }
    return false;
}

struct ast *parser_command(struct token_list *tokens)
{
    struct ast *ast = NULL;
    if (is_shell_command(tokens->current_token))
        ast = parser_shell_command(tokens);
    else
        ast = parser_simple_command(tokens);
    return ast;
}
