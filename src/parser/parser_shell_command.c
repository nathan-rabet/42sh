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
    if (tokens->current_token->type == IF)
        return parser_rule_if(tokens);
    else if (tokens->current_token->type == WHILE)
        return parser_rule_while(tokens);
    else if (tokens->current_token->type == UNTIL)
        return parser_rule_until(tokens);
    else if (tokens->current_token->type == FOR)
        return parser_rule_for(tokens);
    // ERR
    return ast;
}