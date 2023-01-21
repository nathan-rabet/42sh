// GRAMMAR
// rule_while = 'while' compound_list 'do' compound_list 'done' ;

#include "../include/parser.h"

struct ast *parser_rule_while(struct token_list *tokens)
{
    struct ast *condition = NULL;
    struct ast *to_execute = NULL;
    eat(tokens, WHILE);
    condition = parser_compound_list(tokens);
    eat(tokens, DO);
    to_execute = parser_compound_list(tokens);
    eat(tokens, DONE);
    return ast_while_init(condition, to_execute);
}