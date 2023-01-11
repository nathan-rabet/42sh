// GRAMMAR
// list = and_or { ';' | '&' and_or } [ ';' | '&' ] ;

#include "../../include/parser.h"

struct ast *parser_list(token *current_token)
{
    struct ast *ast = NULL;
    ast = parse_and_or(current_token);
    return ast;
}