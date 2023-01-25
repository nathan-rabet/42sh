// GRAMMAR
// element =
//  WORD
//| redirection
//;

#include "../include/parser.h"

char **parser_element(struct token_list *tokens, char **words)
{
    size_t i = 0;
    while (words && words[i] != NULL)
        i++;

    while (tokens->current_token->type == WORD)
    {
        words[i] = xmalloc(1, sizeof(char *));
        words[i++] = tokens->current_token->value;
        eat(tokens, WORD);
    }
    words[i] = NULL;
    return words;
}