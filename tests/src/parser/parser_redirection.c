// GRAMMAR
// redirection = [IONUMBER] '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' WORD ;

#include "../include/parser.h"

struct list_redir *parser_redirection(struct token_list *tokens,
                                      struct list_redir *next)
{
    struct list_redir *ast = NULL;
    char *IONumber = "-1";
    char *target;
    enum ast_redir_type type;

    if (look_ahead(tokens) == IO_NUMBER)
    {
        IONumber = tokens->current_token->value;
        eat(tokens, IO_NUMBER);
    }
    if (tokens->current_token && is_redirection(tokens->current_token->type))
    {
        type = redirection_type(tokens->current_token->type);
        eat(tokens, tokens->current_token->type);
    }
    else
        parser_grammar_return_error_2(tokens->current_token);
    if (look_ahead(tokens) == WORD)
    {
        target = tokens->current_token->value;
        eat(tokens, WORD);
    }
    else
        parser_grammar_return_error_2(tokens->current_token);

    IONumber = strcmp(IONumber, "-1") == 0 ? IONumbertype(type) : IONumber;

    ast = list_redir_init(type, IONumber, target, next);
    return ast;
}
