// GRAMMAR
// simple_command =
// prefix { prefix }
//| { prefix } WORD { element }
//;

#include "../include/parser.h"

struct ast *parser_simple_command(struct token_list *tokens)
{
    struct list_redir *list = NULL;
    char **words = xmalloc(100, sizeof(char *));
    words[0] = NULL;

    while (tokens->current_token && is_prefix(tokens))
    {
        if (is_redirection(tokens->current_token->type))
            list = parser_redirection(tokens, list);
        if (is_assignment_word(tokens->current_token->value))
            words = parser_element(tokens, words);
    }

    if (look_ahead(tokens) == WORD)
    {
        words = parser_element(tokens, words);
    }

    while (tokens->current_token
           && (is_redirection(tokens->current_token->type)
               || tokens->current_token->type == WORD))
    {
        if (is_redirection(tokens->current_token->type))
            list = parser_redirection(tokens, list);
        if (tokens->current_token->type == WORD)
            words = parser_element(tokens, words);
    }
    if (words[0] == NULL)
        return NULL;
    else if (list == NULL)
        return ast_cmd_init(words);
    return ast_redir_init(list, ast_cmd_init(words));
}