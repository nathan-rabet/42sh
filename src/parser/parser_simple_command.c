// GRAMMAR
// simple_command =
//prefix { prefix }
//| { prefix } WORD { element }
//;

#include "../../include/parser.h"

struct ast *parser_simple_command(struct token_list *tokens)
{
    struct ast *ast = NULL;
    char *first = xmalloc(1, sizeof(char *));
    struct list_redir *list = NULL;

    while (is_redirection(tokens->current_token->type))
        list = parser_redirection(tokens, list);


    if (tokens->current_token->type == WORD)
    {
        // Copy the value of the current token
        first = tokens->current_token->value;
        eat(tokens, WORD);
        ast = parser_element(tokens, first);
    }
    else
        return ast_redir_init(list, NULL);

    while (is_redirection(tokens->current_token->type))
            list = parser_redirection(tokens, list);

    if (list == NULL)
        return ast;

    return ast_redir_init(list, ast);
}