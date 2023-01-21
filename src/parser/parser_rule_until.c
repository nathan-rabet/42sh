// GRAMMAR
// rule_until = 'until' compound_list 'do' compound_list 'done' ;

#include "../include/parser.h"

struct ast *parser_rule_until(struct token_list *tokens)
{
    struct ast *condition = NULL;
    struct ast *to_execute = NULL;
    eat(tokens, UNTIL);
    condition = parser_compound_list(tokens);
    eat(tokens, DO);
    to_execute = parser_compound_list(tokens);
    eat(tokens, DONE);
    return ast_until_init(condition, to_execute);
}