// GRAMMAR
// simple_command =
//prefix { prefix }
//| { prefix } WORD { element }
//;

#include "../../include/parser.h"

struct ast *parser_simple_command(struct token *current_token)
{
    struct ast *ast = NULL;
    char *first = xmalloc(1, sizeof(char *));
    if (current_token->type == WORD)
    {
        // Copy the value of the current token
        first = current_token->value;
        current_token = eat(current_token, WORD);
        ast = parser_element(current_token, first);


        /*struct ast_cmd *cmd = (struct ast_cmd *)ast;
        fprintf(stdout, "test %s\n", first);
        cmd->words[0] = first;*/
    }

    return ast;
}