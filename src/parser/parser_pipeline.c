// GRAMMAR
// pipeline = ['!'] command { '|' {'\n'} command } ;

#include "../../include/parser.h"

struct ast *parser_pipeline(struct token_list *tokens)
{
    struct ast **command = xmalloc(1, sizeof (struct ast **));
    size_t i = 1;
    command[0] = parser_command(tokens);

    while (tokens->current_token->type == PIPE)
    {
        eat(tokens, PIPE);
        while (tokens->current_token->type == NEWLINE)
            eat(tokens, NEWLINE);
        if (tokens->current_token)
            command[i++] = parser_command(tokens);
    }
    command[i] = NULL;
    if (i < 2)
        return command[0];
    return ast_pipe_init(i, command);
}