// GRAMMAR
// list = and_or { ';' | '&' and_or } [ ';' | '&' ] ;

#include "../include/parser.h"

struct ast *parser_list(struct token_list *tokens)
{
    struct ast **children = xmalloc(100, sizeof(struct ast **));
    size_t i = 1;
    children[0] = parser_and_or(tokens);

    while (tokens->current_token && tokens->current_token->type == SEMI)
    {
        eat(tokens, SEMI);
        while (look_ahead(tokens) == NEWLINE)
            eat(tokens, NEWLINE);
        if (tokens->current_token)
        {
            children[i] = parser_and_or(tokens);
            if (children[i++] == NULL)
                parser_grammar_return_error_2(tokens->current_token);
        }

    }

    children[i] = NULL;
    // if 1 child
    if (i < 2)
        return children[0];

    return ast_list_init(i, children);
}