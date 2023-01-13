// GRAMMAR
// compound_list = and_or [';'] {'\n'} ;

#include "../../include/parser.h"

struct ast *parser_compound_list(struct token_list *tokens)
{
    struct ast *ast = NULL;
    ast = parser_and_or(tokens);
    if (look_ahead(tokens) == SEMI)
        eat(tokens, SEMI);
    while (look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);
    return ast;
}