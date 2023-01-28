// GRAMMAR
// rule_if = 'if' compound_list 'then' compound_list [else_clause] 'fi' ;

#include "../include/parser.h"

struct ast *parser_rule_if(struct token_list *tokens)
{
    struct ast *ast = NULL;

    // IF
    eat(tokens, IF);
    struct ast *if_condition = parser_compound_list(tokens);
    if (if_condition == NULL)
        parser_grammar_return_error_2(tokens->current_token);
    // THEN
    eat(tokens, THEN);
    struct ast *then = parser_compound_list(tokens);
    if (then == NULL)
        parser_grammar_return_error_2(tokens->current_token);

    // ELSE may be NULL
    struct ast *else_clause = parser_else_clause(tokens);

    eat(tokens, FI);
    // Construct ast
    ast = ast_if_init(if_condition, then, else_clause);

    return ast;
}
