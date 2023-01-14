// GRAMMAR
// list = and_or { ';' | '&' and_or } [ ';' | '&' ] ;

#include "../../include/parser.h"

struct ast *parser_list(struct token_list *tokens)
{
    struct ast* ast = NULL;
    struct ast **children = xmalloc(1, sizeof(struct ast **));
    size_t i = 1;
    children[0] = parser_and_or(tokens);
    // printf("test %i\n", tokens->current_token->type);

    while (tokens->current_token->type == SEMI)
    {

        eat(tokens, SEMI);

        if (tokens->current_token)
            children[i++] = parser_and_or(tokens);
    }

    children[i] = NULL;
    // if 1 chil
    if (i < 2)
        return children[0];

    ast = ast_list_init(i, children);
    return ast;
}