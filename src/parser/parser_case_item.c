// GRAMMAR
// case_item = ['('] WORD { '|' WORD } ')' {'\n'} [compound_list] ;

#include "../../include/parser.h"

struct list_case_item *parser_case_item(struct token_list *tokens)
{
    char **word = xmalloc(1, sizeof(char **));
    size_t i = 0;
    struct ast *to_execute = NULL;

    if (strcmp(tokens->current_token->value, "(") == 0)
        eat(tokens, WORD);

    if (look_ahead(tokens) == WORD)
    {
        word[i++] = tokens->current_token->value;
        eat(tokens, WORD);
    }

    while (look_ahead(tokens) == PIPE)
    {
        eat(tokens, PIPE);
        word[i++] = tokens->current_token->value;
        eat(tokens, WORD);
    }

    if (strcmp(tokens->current_token->value, ")") == 0)
        eat(tokens, WORD);

    while (look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);

    to_execute = parser_compound_list(tokens);

    return list_case_item_init(word, i, to_execute);
}
