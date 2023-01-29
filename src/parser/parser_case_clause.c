// GRAMMAR
// case_clause = case_item { ';;' {'\n'} case_item } [';;'] {'\n'} ;

#include "../include/parser.h"

struct ast *parser_case_clause(struct token_list *tokens)
{
    struct list_case_item **case_item =
        xmalloc(100, sizeof(struct list_case_item **));
    size_t i = 0;
    char *name = NULL;

    case_item[i++] = parser_case_item(tokens);

    while (look_ahead(tokens) == DSEMI)
    {
        eat(tokens, DSEMI);
        while (look_ahead(tokens) == NEWLINE)
            eat(tokens, NEWLINE);
        if (look_ahead(tokens) != ESAC)
            case_item[i++] = parser_case_item(tokens);
    }

    if (look_ahead(tokens) == DSEMI)
        eat(tokens, DSEMI);

    while (look_ahead(tokens) == NEWLINE)
        eat(tokens, NEWLINE);

    return ast_case_init(name, case_item, i);
}
