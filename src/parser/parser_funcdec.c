// GRAMMAR
// funcdec = WORD '(' ')' {'\n'} shell_command ;

#include "../../include/parser.h"


struct ast *parser_funcdec(struct token_list *tokens)
{
    char *name;
    struct ast *to_execute;

    name = tokens->current_token->value;
    eat(tokens, WORD);

    if (strcmp(tokens->current_token->value, "(") == 0
    && strcmp(tokens->current_token->next->value, ")") == 0)
    {
        eat(tokens, WORD);
        eat(tokens, WORD);
    }
    else
        parser_grammar_return_error_2(tokens->current_token);

    while (look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);

    to_execute = parser_shell_command(tokens);

    return ast_func_init(name, to_execute);
}
