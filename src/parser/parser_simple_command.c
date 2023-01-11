// GRAMMAR
// simple_command =
//prefix { prefix }
//| { prefix } WORD { element }
//;

#include "../../include/parser.h"

struct ast *parser_simple_command(token *current_token)
{
    struct ast *ast = NULL;

}