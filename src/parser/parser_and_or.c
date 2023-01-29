// GRAMMAR
// pipeline { '&&' | '||' {'\n'} pipeline } ;

#include "../include/parser.h"

struct ast *parser_and_or(struct token_list *tokens)
{
    struct ast *ast = NULL;
    struct ast *ast_right = NULL;

    ast = parser_pipeline(tokens);

    while (tokens->current_token
           && (tokens->current_token->type == AND_IF
               || tokens->current_token->type == OR_IF))
    {
        if (ast == NULL)
            parser_grammar_return_error_2(tokens->current_token);
        enum token_types type = tokens->current_token->type;
        eat(tokens, type);
        while (look_ahead(tokens) == NEWLINE)
            eat(tokens, NEWLINE);
        if (type == AND_IF)
        {
            if ((ast_right = parser_pipeline(tokens)) == NULL)
                parser_grammar_return_error_2(tokens->current_token);
            ast = ast_and_init(ast, ast_right);
        }
        else if (type == OR_IF)
        {
            if ((ast_right = parser_pipeline(tokens)) == NULL)
                parser_grammar_return_error_2(tokens->current_token);
            ast = ast_or_init(ast, ast_right);
        }
        else
            parser_grammar_return_error_2(tokens->current_token);
    }

    return ast;
}
