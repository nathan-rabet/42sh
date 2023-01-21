// GRAMMAR
// compound_list = {'\n'} and_or { ';' | '\n' {'\n'} and_or } [';'] {'\n'} ;

#include "../include/parser.h"

struct ast *parser_compound_list(struct token_list *tokens)
{
    struct ast *ast = NULL;
    struct ast **children = xmalloc(100, sizeof(struct ast **));
    size_t i = 1;

    while (look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);
    children[0] = parser_and_or(tokens);
    while(look_ahead(tokens) == NEWLINE || look_ahead(tokens) == SEMI)
    {
        if (look_ahead(tokens) == SEMI)
           eat(tokens, SEMI);

        while (look_ahead(tokens) == NEWLINE)
           eat(tokens, NEWLINE);

        children[i++] = parser_and_or(tokens);
    }

    if (look_ahead(tokens) == SEMI)
        eat(tokens, SEMI);
    while (look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);
    children[i] = NULL;

    // if 1 child
    if (i < 2)
        return children[0];
    ast = ast_list_init(i, children);
    return ast;
}