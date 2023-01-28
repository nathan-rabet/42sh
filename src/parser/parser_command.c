// GRAMMAR
// command =
// simple_command
//| shell_command { redirection }
//| funcdec { redirection }
//;

#include "../include/parser.h"

struct ast *parser_command(struct token_list *tokens)
{
    struct ast *ast = NULL;
    if (tokens->current_token == NULL)
        return NULL;
    if (is_shell_command(tokens->current_token))
    {
        ast = parser_shell_command(tokens);
        struct list_redir *list = NULL;

        while (tokens->current_token != NULL
               && is_redirection(tokens->current_token->type))
            list = parser_redirection(tokens, list);
        if (list != NULL)
            return ast_redir_init(list, ast);
    }
    else if (is_funcdec(tokens))
    {
        ast = parser_funcdec(tokens);
        struct list_redir *list = NULL;

        while (is_redirection(tokens->current_token->type))
            list = parser_redirection(tokens, list);
        if (list != NULL)
            return ast_redir_init(list, ast);
    }
    else
        return parser_simple_command(tokens);
    return ast;
}
