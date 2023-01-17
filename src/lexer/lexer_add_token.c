#include "lexer.h"

void token_add(lexer *lex, token_t token_type, quote_t quote_type,
               size_t value_size)
{
    struct token *new = xmalloc(1, sizeof(token));

    new->type = token_type;
    new->quote = quote_type;
    new->value = xcalloc(1, value_size);
    memcpy(new->value, lex->str_token_start, value_size);
    new->next = NULL;

    if (lex->tokens == NULL)
        lex->tokens = new;
    else
    {
        for (struct token *tmp = lex->tokens; tmp != NULL; tmp = tmp->next)
        {
            if (tmp->next == NULL)
            {
                tmp->next = new;
                break;
            }
        }
    }

    lex->str_token_start = lex->str_token_end + 1; // +1 for the loop increment.
}
