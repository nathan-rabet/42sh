// GRAMMAR
// input =
// list '\n'
//| list EOF
//| '\n'
//| EOF

#include "../include/parser.h"

struct ast *parser_input(struct token *list_tokens)
{
    if (list_tokens == NULL)
        return NULL;
    struct ast *ast = NULL;
    struct token_list *tokens = xmalloc(1, sizeof(struct tokens *));
    tokens->current_token = list_tokens;

    while (look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);

    ast = parser_list(tokens);
    // printf("input %i\n", tokens->current_token->type);
    if (tokens->current_token == NULL)
        return ast;

    while (look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);

    if (tokens->current_token != NULL)
        wrong_look_ahead(tokens, "parse_input");

    return ast;
}