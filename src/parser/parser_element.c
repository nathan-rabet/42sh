// GRAMMAR
// element =
//WORD
//| redirection
//;

#include "../../include/parser.h"

struct ast *parser_element(struct token *current_token, char *first)
{
    char **names = xmalloc(1, sizeof (char **));
    *names = xmalloc(2, sizeof(char *));
    size_t i = 1;
    names[0] = first;

    while (current_token->type == WORD)
    {
        xrealloc(names, i + 1, sizeof (char *));
        names[i] = current_token->value;
        current_token = eat(current_token, WORD);
        i++;
    }
    names[i] = NULL;

    return ast_cmd_init(names);
}