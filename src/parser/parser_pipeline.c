// GRAMMAR
// pipeline = ['!'] command { '|' {'\n'} command } ;

#include "../../include/parser.h"

struct ast *parser_pipeline(token *current_token)
{
    struct ast *ast = NULL;
}