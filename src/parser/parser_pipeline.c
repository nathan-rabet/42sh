// GRAMMAR
// pipeline = ['!'] command { '|' {'\n'} command } ;

#include "../include/parser.h"

struct ast *parser_pipeline(struct token_list *tokens)
{
    struct ast **command = xmalloc(100, sizeof(struct ast **));
    size_t i = 1;
    bool negation = false;
    if (look_ahead(tokens) == BANG)
    {
        eat(tokens, BANG);
        negation = true;
    }

    command[0] = parser_command(tokens);

    while (tokens->current_token && look_ahead(tokens) == PIPE)
    {
        if (command[0] == NULL)
            parser_grammar_return_error_2(tokens->current_token);
        eat(tokens, PIPE);
        while (look_ahead(tokens) == NEWLINE)
            eat(tokens, NEWLINE);
        if (tokens->current_token)
            command[i++] = parser_command(tokens);
        else
            parser_grammar_return_error_2(tokens->current_token);
    }
    command[i] = NULL;
    if (i < 2 && !negation)
        return command[0];
    else if (i < 2 && negation)
        return ast_not_init(command[0]);
    else if (negation)
        return ast_not_init(ast_pipe_init(i, command));
    return ast_pipe_init(i, command);
}
