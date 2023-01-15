// GRAMMAR
// pipeline { '&&' | '||' {'\n'} pipeline } ;

#include "../../include/parser.h"

struct ast *parser_and_or(struct token_list *tokens)
{
    struct ast *ast = NULL;
    ast = parser_pipeline(tokens);

    while (tokens->current_token->type == AND_IF || tokens->current_token->type == OR_IF)
    {
        enum token_types type = tokens->current_token->type;
        eat(tokens, type);
        while (tokens->current_token->type == NEWLINE)
            eat(tokens, NEWLINE);
        if (type == AND_IF)
            ast = ast_and_init(ast, parser_pipeline(tokens));
        else if (type == OR_IF)
            ast = ast_or_init(ast, parser_pipeline(tokens));
    }

    return ast;
}
