// GRAMMAR
// pipeline { '&&' | '||' {'\n'} pipeline } ;

#include "../../include/parser.h"

struct ast *parser_and_or(struct token *current_token)
{
    struct ast *ast = NULL;
    ast = parser_pipeline(current_token);
    return ast;
}
