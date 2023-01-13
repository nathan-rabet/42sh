// GRAMMAR
// pipeline { '&&' | '||' {'\n'} pipeline } ;

#include "../../include/parser.h"

struct ast *parser_and_or(struct token_list *tokens)
{
    struct ast *ast = NULL;
    ast = parser_pipeline(tokens);
    return ast;
}
