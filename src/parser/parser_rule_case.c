// GRAMMAR
// rule_case = 'case' WORD {'\n'} 'in' {'\n'} [case_clause] 'esac' ;

#include "../include/parser.h"

struct ast *parser_rule_case(struct token_list *tokens)
{
    struct ast_case *ast = NULL;
    char *name = NULL;

    eat(tokens, CASE);

    if (look_ahead(tokens) != WORD)
        parser_grammar_return_error_2(tokens->current_token);
    name = tokens->current_token->value;
    eat(tokens, WORD);

    while (look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);

    eat(tokens, IN);
    while (look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);
    if (look_ahead(tokens) != ESAC)
    {
        ast = (struct ast_case *)parser_case_clause(tokens);
        if (ast)
            ast->name = name;
    }
    eat(tokens, ESAC);
    return (struct ast *)ast;
}
