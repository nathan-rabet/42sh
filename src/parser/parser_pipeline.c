// GRAMMAR
// pipeline = ['!'] command { '|' {'\n'} command } ;

#include "../../include/parser.h"

struct ast *parser_pipeline(struct token_list *tokens)
{
    struct ast *ast = NULL;
    ast = parser_command(tokens);
    return ast;
}