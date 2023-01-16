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
    if (look_ahead(tokens) == IF)
        return parser_rule_if(tokens);
    else if (look_ahead(tokens) == WHILE)
        return parser_rule_while(tokens);
    else if (look_ahead(tokens) == UNTIL)
        return parser_rule_until(tokens);
    else if (look_ahead(tokens) == FOR)
        return parser_rule_for(tokens);
    else if (look_ahead(tokens) == LBRACE)
    {
        eat(tokens, LBRACE);
        ast = ast_brace_init(parser_compound_list(tokens));
        eat(tokens, RBRACE);
    }
    // ERR
    return ast;
}