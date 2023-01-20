#include <assert.h>
#include <string.h>

#include "../../include/lexer.h"
#include "../../include/xalloc.h"

void token_add(lexer *lex, token_t token_type, size_t value_size)
{
    assert(value_size <= GET_LEN_CURRENT_CHAR(lex));

    if (value_size == 0)
        return;

    struct token *new = xmalloc(1, sizeof(token));

    new->type = token_type;
    new->value = xcalloc(value_size + 1, sizeof(char));
    strncpy(new->value, lex->str_token_start, value_size);
    new->next = NULL;

    if (lex->tokens == NULL)
        lex->tokens = new;
    else
        for (struct token *tmp = lex->tokens; tmp != NULL; tmp = tmp->next)
            if (tmp->next == NULL)
            {
                tmp->next = new;
                break;
            }

    lex->str_token_start += value_size;
}
