// GRAMMAR
// input =
// list '\n'
//| list EOF
//| '\n'
//| EOF

#include "../include/parser.h"

struct ast *parser_input(struct token_list *tokens)
{
    struct ast *ast = NULL;

    while (look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);

    ast = parser_list(tokens);
    // printf("input %i\n", tokens->current_token->type);
    if (tokens->current_token == NULL)
        return ast;

    while (look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);

    // printf("value : %s",tokens->current_token->value);
    // if (tokens->current_token != NULL)
    // wrong_look_ahead(tokens, "parse_input");

    return ast;
}
