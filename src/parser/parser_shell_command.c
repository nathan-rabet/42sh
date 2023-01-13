// GRAMMAR
// shell_command =
//'{' compound_list '}'
//| '(' compound_list ')'
//| rule_for
//| rule_while
//| rule_until
//| rule_case
//| rule_if
//;

#include "../../include/parser.h"

struct ast *parser_shell_command(struct token_list *tokens)
{
    struct ast *ast = NULL;
    ast = parser_rule_if(tokens);
    return ast;
}