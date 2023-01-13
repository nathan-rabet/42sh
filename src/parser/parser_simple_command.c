// GRAMMAR
// simple_command =
//prefix { prefix }
//| { prefix } WORD { element }
//;

#include "../../include/parser.h"

struct ast *parser_simple_command(struct token_list *tokens)
{
    struct ast *ast = NULL;
    char *first = xmalloc(1, sizeof(char *));
    if (tokens->current_token->type == WORD)
    {
        // Copy the value of the current token
        first = tokens->current_token->value;
        eat(tokens, WORD);
        ast = parser_element(tokens, first);


        /*struct ast_cmd *cmd = (struct ast_cmd *)ast;
        fprintf(stdout, "test %s\n", first);
        cmd->words[0] = first;*/
    }

    return ast;
}