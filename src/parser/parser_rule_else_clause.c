// GRAMMAR
// else_clause =
//'else' compound_list
//| 'elif' compound_list 'then' compound_list [else_clause]
//;

#include "../include/parser.h"

struct ast *parser_else_clause(struct token_list *tokens)
{
    struct ast *ast = NULL;
    if (look_ahead(tokens) == ELSE)
    {
        eat(tokens, ELSE);
        ast = parser_compound_list(tokens);
        if (ast == NULL)
            parser_grammar_return_error_2(tokens->current_token);
    }
    else if (look_ahead(tokens) == ELIF)
    {
        ast = parser_rule_elif(tokens);
        if (ast == NULL)
            parser_grammar_return_error_2(tokens->current_token);
    }

    return ast;
}
