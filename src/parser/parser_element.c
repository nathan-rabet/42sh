// GRAMMAR
// element =
//  WORD
//| redirection
//;

#include "../../include/parser.h"

struct ast *parser_element(struct token_list *tokens, char *first)
{
        char **names = xmalloc(1, sizeof (char **));
        *names = xmalloc(2, sizeof(char *));
        size_t i = 1;
        names[0] = first;
        if (first == NULL)
            i = 0;

        while (tokens->current_token->type == WORD)
        {
            xrealloc(names, i + 1, sizeof (char *));
            names[i++] = tokens->current_token->value;
            eat(tokens, WORD);
        }
        names[i] = NULL;
        return ast_cmd_init(names);
}