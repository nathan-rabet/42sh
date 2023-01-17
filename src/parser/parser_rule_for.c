// GRAMMAR
// rule_for =
//'for' WORD [';'] [ {'\n'} 'in' { WORD } ';' | '\n' ] {'\n'} 'do' compound_list 'done' ;

#include "../../include/parser.h"

struct ast *parser_rule_for(struct token_list *tokens)
{
    char *name = NULL;
    char **names = xmalloc(1, sizeof (char **));
    struct ast *words = NULL;
    struct ast *to_execute = NULL;

    eat(tokens, FOR);
    name = tokens->current_token->value;
    eat(tokens, WORD);

    if (look_ahead(tokens) == SEMI)
        eat(tokens, SEMI);

    while(look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);

    if (look_ahead(tokens) == IN)
    {
        eat(tokens, IN);
        words = ast_cmd_init(parser_element(tokens, names));
        if (look_ahead(tokens) == SEMI)
            eat(tokens, SEMI);
        else if (look_ahead(tokens) == NEWLINE)
            eat(tokens, NEWLINE);
        else
            parser_grammar_return_error_2(tokens->current_token);
    }

    while(look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);

    eat(tokens, DO);

    to_execute = parser_compound_list(tokens);

    eat(tokens, DONE);
    return ast_for_init(name, words, to_execute);
}