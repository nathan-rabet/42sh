// GRAMMAR
// pipeline = ['!'] command { '|' {'\n'} command } ;

#include "../../include/parser.h"

struct ast *parser_pipeline(struct token *current_token)
{
    struct ast *ast = NULL;
    ast = parser_command(current_token);
    return ast;
}