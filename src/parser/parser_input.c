// GRAMMAR
// input =
// list '\n'
//| list EOF
//| '\n'
//| EOF

#include "../include/parser.h"

struct token *add_Newline(struct token *tmp)
{
    struct token *list = tmp;
    if (tmp == NULL)
        return NULL;
    while (list->next != NULL)
        list = list->next;
    if (list->type == NEWLINE)
        return tmp;
    struct token *token = xmalloc(1, sizeof(struct token));
    token->type = NEWLINE;
    token->next = NULL;
    list->next = token;
    return tmp;
}

struct ast *parser_input(struct token *list_tokens)
{
    struct ast *ast = NULL;
    struct token_list *tokens = xmalloc(1, sizeof(struct tokens *));
    tokens->current_token = add_Newline(list_tokens);
    if (list_tokens == NULL)
        return NULL;
    else if (tokens->current_token->type == NEWLINE)
        eat(tokens, NEWLINE);

    ast = parser_list(tokens);
    // printf("input %i\n", tokens->current_token->type);
    if (tokens->current_token == NULL)
        return ast;
    switch (tokens->current_token->type)
    {
    case NEWLINE:
        eat(tokens, NEWLINE);
        break;
    default:
        wrong_look_ahead(tokens, "parse_input");
    }

    return ast;
}